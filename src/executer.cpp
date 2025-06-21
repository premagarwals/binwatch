#include <iostream>
#include "executer.h"
#include <unistd.h>

using namespace std;

Executor::Executor() {}

int Executor::execute(const std::string& binary, char* const argv[], double& runtime,
                      int maxMem, int maxTime, int nobody_uid, const std::string& chrootDir,
                      size_t* peakMemoryOut, bool* killedOut, int* exitCodeOut,
                      std::vector<size_t>* memSamplesOut) {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << '\n';
        return -1;
    } else if (pid == 0) {
        cout << "Executed: " << binary << '\n';
    } else {
        cout << "Child PID: " << pid << '\n';
    }
}
