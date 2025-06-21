#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    string binary; // Path to the executable
    int maxMem = 32768; // Default memory limit is approx 32MB
    int maxTime = 10;  //Default time limit is 10 seconds
    string chrootDir; //Sandbox directory
    int nobody_uid = 65534; // Using this UID to remove all permissions

    vector<std::string> binary_with_args;
    
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--exe") == 0 || strcmp(argv[i], "-e") == 0) && i + 1 < argc) {
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
        } else if ((strcmp(argv[i], "--chroot") == 0 || strcmp(argv[i], "-c") == 0) && i + 1 < argc) {
            chrootDir = argv[++i];
        } else if ((strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)) {
            cout << "Usage: " << argv[0] << " [--max-mem <KB>] [--max-time <sec>] [--chroot <dir>] --exe <path> [args...]\n";
            return 0;
        }
    }

    if (binary.empty()) {
        cerr << "Error: --exe is required.\n";
        return 1;
    }

    return 0;
}