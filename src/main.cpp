#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

#include "executer.h"

using namespace std;

void printMemoryGraph(const vector<size_t>& memSamples);

int main(int argc, char* argv[]) {
    string binary; // Path to the executable
    size_t maxMem = 32768; // Default memory limit is approx 32MB
    int maxTime = 10;  //Default time limit is 10 seconds
    string chrootDir; //Sandbox directory
    int nobody_uid = 65534; // Using this UID to remove all permissions
    bool sandbox = false; // disable sandboxing by default

    vector<string> binary_with_args;
    
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-exe") == 0 || strcmp(argv[i], "-e") == 0) && i + 1 < argc) {
            for (int j = i + 1; j < argc; ++j) {
                if (
                    strcmp(argv[j], "--max-mem") == 0 || strcmp(argv[j], "-m") == 0 ||
                    strcmp(argv[j], "--max-time") == 0 || strcmp(argv[j], "-t") == 0 ||
                    strcmp(argv[j], "--chroot") == 0 || strcmp(argv[j], "-c") == 0 ||
                    strcmp(argv[j], "--help") == 0 || strcmp(argv[j], "-h") == 0
                ) break;
                binary_with_args.push_back(argv[j]);
            }
            if (!binary_with_args.empty()) {
                binary = binary_with_args[0];
            }
            break;
        } else if ((strcmp(argv[i], "--max-mem") == 0 || strcmp(argv[i], "-m") == 0) && i + 1 < argc) {
            maxMem = stoi(argv[++i]);
        } else if ((strcmp(argv[i], "--max-time") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc) {
            maxTime = stoi(argv[++i]);
        } else if ((strcmp(argv[i], "--sandbox") == 0 || strcmp(argv[i], "-s") == 0) && i + 1 < argc) {
            sandbox = true;
        } else if ((strcmp(argv[i], "--chroot") == 0 || strcmp(argv[i], "-c") == 0) && i + 1 < argc) {
            sandbox = true;
            chrootDir = argv[++i];
        } else if ((strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)) {
            cout << "\nUsage: \n\t" << argv[0] << " [--max-mem <KB>] [--max-time <sec>] -exe <command> [args...]\n";
            cout << "\tsudo " << argv[0] << " [--max-mem <KB>] [--max-time <sec>] [--sandbox] -exe <command> [args...]\n";
            cout << "\tsudo " << argv[0] << " [--max-mem <KB>] [--max-time <sec>] [--chroot <path>] -exe <command> [args...]\n";
            cout << "\n\t--sandbox will drop the privileges of the process but will run in default directory.\n";
            cout << "\t--chroot will drop the privileges of the process and run in the specified directory.\n";
            cout << "\n\te.g. sudo " << argv[0] << " --max-mem 16384 --max-time 5 --chroot /path/to/sandbox -exe ls -l\n";
            return 0;
        }
    }

    if (binary.empty()) {
        cerr << "Error: -exe is required.\n";
        cerr << "Use --help or -h for usage information.\n";
        return 1;
    }

    vector<char*> target_argv;
    for (const auto& arg : binary_with_args) {
        target_argv.push_back(const_cast<char*>(arg.c_str()));
    }
    target_argv.push_back(nullptr);

    Executor executor;
    double runtime = 0.0;
    size_t peakMemory = 0;
    int exitCode = 0;
    bool killed = false;
    int PID = 0;
    vector<size_t> memSamples;

    executor.execute(
        binary,
        target_argv.data(),
        runtime,
        maxMem,
        maxTime,
        nobody_uid,
        sandbox,
        chrootDir,
        &peakMemory,
        &killed,
        &exitCode,
        &memSamples
    );
    cout << "\n:::::::::::::::::::: EXECUTION SUMMARY :::::::::::::::::::\n";
    cout << "\nPID: " << PID << "\n"
              << "Runtime: " << runtime << "s\n"
              << "Peak Memory: " << peakMemory << " KB\n"
              << "Exit Code: " << exitCode << "\n"
              << "Killed: " << (killed ? "true" : "false") << "\n";
    
    printMemoryGraph(memSamples);
    return 0;
}

void printMemoryGraph(const vector<size_t>& memSamples) {
    size_t maxWidth = 50;
    if (memSamples.empty()) {
        cout << "No memory samples collected.\n";
        return;
    }
    size_t maxMem = *max_element(memSamples.begin(), memSamples.end());
    if (maxMem == 0) maxMem = 1;

    cout << "\nMemory usage graph (in KB):\n";
    for (size_t mem : memSamples) {
        size_t barLen = (mem * maxWidth) / maxMem;
        cout << "|";
        for (size_t i = 0; i < barLen; ++i) cout << "█";
        for (size_t i = barLen; i < maxWidth; ++i) cout << " ";
        cout << "| " << mem << " KB\n";
    }
}