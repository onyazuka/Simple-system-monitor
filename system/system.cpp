#include "system.hpp"

#ifdef __linux__

/*
    returns true on success, false on failure
*/
bool LinuxSystem::terminateProcess(PID pid)
{
    int err = kill(pid, SIGTERM);
    if(err)
    {
        return false;
    }
    return true;
}

/*
    returns true on success, false on failure
*/
bool LinuxSystem::killProcess(PID pid)
{
    int err = kill(pid, SIGKILL);
    if(err)
    {
        return false;
    }
    return true;
}

#endif

System::pointer SystemGetter::getSystem()
{
#ifdef __linux__
    return LinuxSystem::pointer(new LinuxSystem());
#else
    // other variants
    return nullptr;
#endif
}
