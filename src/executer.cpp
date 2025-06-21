#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <signal.h>

#include "executer.h"
#include "sandbox.h"

using namespace std;

Executor::Executor() {}

int Executor::execute(const std::string& binary, char* const argv[], double& runtime,
                      int maxMem, int maxTime, int nobody_uid, bool sandbox, const string& chrootDir,
                      size_t* peakMemoryOut, bool* killedOut, int* exitCodeOut,
                      std::vector<size_t>* memSamplesOut) {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return -1;
    } else if (pid == 0) {
        if (sandbox) Sandbox::setupSandbox(
            chrootDir.empty() ? nullptr : chrootDir.c_str(),
            nobody_uid,
            maxMem,
            maxTime
        );
        execvp(binary.c_str(), argv);
        cerr << "Execution failed for: " << binary << endl;
        _exit(EXIT_FAILURE);
    } else {
        auto start = std::chrono::steady_clock::now();
        size_t peakMemory = 0;
        bool killed = false;
        int exitCode = -1;
        int status = 0;

        vector<size_t> mem_samples;

        while (true) {
            pid_t result = waitpid(pid, &status, WNOHANG);
            string statm_path = "/proc/" + to_string(pid) + "/statm";
            ifstream statm(statm_path);
            size_t rss = 0;
            if (statm) {
                size_t dummy;
                statm >> dummy >> rss;
                size_t mem_kb = rss * (getpagesize() / 1024);
                mem_samples.push_back(mem_kb);
                if (mem_kb > peakMemory) peakMemory = mem_kb;
            }

            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(now - start).count();
            if (maxTime > 0 && elapsed > maxTime) {
                kill(pid, SIGKILL);
                killed = true;
                waitpid(pid, &status, 0);
                break;
            }

            if (result == 0) {
                this_thread::sleep_for(chrono::milliseconds(100));
            } else {
                break;
            }
        }
        auto end = chrono::steady_clock::now();
        runtime = chrono::duration<double>(end - start).count();

        if (!killed) {
            if (WIFEXITED(status)) {
                exitCode = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                killed = true;
                exitCode = WTERMSIG(status);
            }
        }

        if (peakMemoryOut) *peakMemoryOut = peakMemory;
        if (killedOut) *killedOut = killed;
        if (exitCodeOut) *exitCodeOut = exitCode;
        if (memSamplesOut) *memSamplesOut = mem_samples;

        return exitCode;
    }
}
