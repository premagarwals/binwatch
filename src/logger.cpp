#include <fstream>
#include <iostream>
#include <string>

#include "logger.h"

using namespace std;

Logger::Logger(const string& logFile) : logFileName(logFile) {}

void Logger::logExecutionDetails(const string& binary, int pid, double runtime, size_t peakMemory, int exitCode, bool killed) {
    ofstream logFile(logFileName, ios::app);
    if (logFile.is_open()) {
        logFile << "\nExecuted binary: " << binary << "\n"
                << "PID: " << pid << "\n"
                << "Runtime: " << runtime << "s\n"
                << "Peak Memory: " << peakMemory << " KB\n"
                << "Exit Code: " << exitCode << "\n"
                << "Killed: " << (killed ? "true" : "false") << "\n"
                << "-------------------------\n";
        logFile.close();
    } else {
        cerr << "Unable to open log file: " << logFileName << endl;
    }
}
