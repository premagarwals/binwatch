#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <vector>

class Executor {
public:
    Executor();
    int execute(const std::string& binaryPath,
                char* const argv[],
                double& runtime,
                int maxMem,
                int maxTime,
                int nobody_uid,
                const std::string& chrootDir,
                size_t* peakMemory = nullptr,
                bool* killed = nullptr,
                int* exitCodeOut = nullptr,
                std::vector<size_t>* memSamplesOut = nullptr);
};

#endif
