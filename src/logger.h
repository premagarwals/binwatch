#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

using namespace std;

class Logger {
public:
    Logger(const string& logFilePath);
    void logExecutionDetails(const string& binary, int pid, double runtime, size_t peakMemory, int exitCode, bool killed);

private:
    fstream logFile;
    string logFileName;
};

#endif
