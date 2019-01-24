#pragma once
#include <memory>
#include "applicationnamespace.hpp"

#ifdef __linux__
#include <sys/types.h>
#include <signal.h>
#endif

/*
    Abstract class for calling system functions.
    Some system functions may also be called in parsers.
*/
class System
{
public:
    typedef std::shared_ptr<System> pointer;
    virtual bool terminateProcess(PID pid) = 0;
    virtual bool killProcess(PID pid) = 0;
};

#ifdef __linux__
class LinuxSystem : public System
{
public:
    typedef std::shared_ptr<LinuxSystem> pointer;
    bool terminateProcess(PID pid);
    bool killProcess(PID pid);
};
#endif

// fabrique of system instances
class SystemGetter
{
public:
    static System::pointer getSystem();
private:
    SystemGetter();
};
