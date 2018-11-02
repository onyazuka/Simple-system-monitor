#pragma once
#include <stdint.h>
#include <string>
#include <fstream>
#include <sys/sysinfo.h>
#include "parseerror.hpp"

/*
    Main memory info
*/
struct MemInfo
{
    MemInfo(const std::string& memInfoFile = "/proc/meminfo");
    void update();
    static MemInfo fromMemInfoFile(const std::string& infoFile = "/proc/meminfo");
    inline float memoryUsage() const {return (float)(memTotal - memAvailable) / memTotal * 100;}
    inline float swapUsage() const {return (float)(swapTotal - swapFree) / swapTotal * 100;}
    inline const std::string& getMemInfoFileName() const {return memInfoFile;}
    inline void setMemInfoFileName(const std::string& _memInfoFile) {memInfoFile = _memInfoFile;}
    uint64_t memTotal;
    uint64_t memAvailable;
    uint64_t swapTotal;
    uint64_t swapFree;

private:
    void parseMemInfoFile(const std::string& infoFile = "/proc/meminfo");
    std::string memInfoFile;
};
