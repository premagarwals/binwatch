#include "executer.h"

Executor::Executor() {}

int Executor::execute(const std::string& binaryPath,
                      char* const argv[],
                      double& runtime,
                      int maxMem,
                      int maxTime,
                      int nobody_uid,
                      const std::string& chrootDir,
                      size_t* peakMemory,
                      bool* killed,
                      int* exitCodeOut,
                      std::vector<size_t>* memSamplesOut) {
    if (runtime) runtime = 0.0;
    if (peakMemory) *peakMemory = 0;
    if (killed) *killed = false;
    if (exitCodeOut) *exitCodeOut = 0;
    if (memSamplesOut) memSamplesOut->clear();
    return 0;
}