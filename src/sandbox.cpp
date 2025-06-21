#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <iostream>

#include "sandbox.h"

using namespace std;

bool Sandbox::setupSandbox(const char* chrootPath, uid_t uid, rlim_t maxMemory, rlim_t maxCpuTime) {
    struct rlimit memLimit;
    memLimit.rlim_cur = memLimit.rlim_max = maxMemory * 1024;
    if (setrlimit(RLIMIT_AS, &memLimit) != 0) {
        cerr << "Failed to set memory limit\n";
        return false;
    }

    struct rlimit cpuLimit;
    cpuLimit.rlim_cur = cpuLimit.rlim_max = maxCpuTime;
    if (setrlimit(RLIMIT_CPU, &cpuLimit) != 0) {
        cerr << "Failed to set CPU time limit\n";
        return false;
    }

    if (chrootPath && chrootPath[0]) {
        if (chroot(chrootPath) != 0) {
            cerr << "Failed to chroot to " << chrootPath << "\n";
            return false;
        }
        if (chdir("/") != 0) {
            cerr << "Failed to chdir after chroot\n";
            return false;
        }
    }

    if (setuid(uid) != 0) {
        cerr << "Failed to drop privileges\n";
        return false;
    }

    return true;
}
