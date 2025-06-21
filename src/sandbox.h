#ifndef SANDBOX_H
#define SANDBOX_H

#include <sys/resource.h>
#include <unistd.h>

class Sandbox {
public:
    static bool setupSandbox(const char* chrootPath, uid_t uid, rlim_t maxMemory, rlim_t maxCpuTime);
};

#endif
