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
        cerr << "Fork failed" << endl;
        return -1;
    } else if (pid == 0) {
        execvp(binary.c_str(), argv);
        cerr << "Execution failed for: " << binary << endl;
        _exit(EXIT_FAILURE);
    } else {
        cout << "Child PID: " << pid << endl;
        return 0;
    }
}
