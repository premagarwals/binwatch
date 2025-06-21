#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <thread>
#include <vector>
#include <signal.h>

#include "executer.h"

using namespace std;

Executor::Executor() {}

int Executor::execute(const std::string& binary, char* const argv[], double& runtime,
                      int maxMem, int maxTime, int nobody_uid, const std::string& chrootDir,
                      size_t* peakMemoryOut, bool* killedOut, int* exitCodeOut,
                      std::vector<size_t>* memSamplesOut) {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return -1;
    } else if (pid == 0) {
        execvp(binary.c_str(), argv);
        cerr << "Execution failed for: " << binary << endl;
        _exit(EXIT_FAILURE);
    } else {
        auto start = std::chrono::steady_clock::now();

        bool killed = false;
        int exitCode = -1;
        int status = 0;

        while (true) {
            pid_t result = waitpid(pid, &status, WNOHANG);

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

        if (killedOut) *killedOut = killed;
        if (exitCodeOut) *exitCodeOut = exitCode;

        return exitCode;
    }
}
