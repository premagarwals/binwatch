#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <vector>

using namespace std;

class Executor {
public:
    Executor();
    int execute(const string& binaryPath,
                char* const argv[],
                double& runtime,
                size_t maxMem,
                int maxTime,
                int nobody_uid,
                bool sandbox,
                const string& chrootDir,
                size_t* peakMemory = nullptr,
                bool* killed = nullptr,
                int* exitCodeOut = nullptr,
                vector<size_t>* memSamplesOut = nullptr);
};

#endif
