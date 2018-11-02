#include "meminfo.hpp"

/*
    Creating from /proc/meminfo file
*/
MemInfo::MemInfo(const std::string& infoFile)
    : memInfoFile{infoFile}
{
    parseMemInfoFile(memInfoFile);
}

MemInfo MemInfo::fromMemInfoFile(const std::string& infoFile)
{
    MemInfo newMemInfo(infoFile);
    return newMemInfo;
}

void MemInfo::update()
{
    parseMemInfoFile(memInfoFile);
}

/*
    Returns 0 if everything is ok, else 1.
    Fills passed meminfo struct
*/
void MemInfo::parseMemInfoFile(const std::string& memInfoFile)
{
    std::ifstream ifs{memInfoFile};
    if(!ifs)
    {
        throw MemoryParseError("MemoryInfo::parseMemInfoFile(): can not open file");
    }
    std::string key, value;
    bool parsed = false;
    while(ifs)
    {
        ifs >> key;
        if(key == "MemTotal:")
        {
            parsed = true;
            ifs >> value;
            memTotal = std::stoull(value);
        }
        else if (key == "MemAvailable:")
        {
            parsed = true;
            ifs >> value;
            memAvailable = std::stoull(value);
        }
        else if(key == "SwapTotal:")
        {
            parsed = true;
            ifs >> value;
            swapTotal = std::stoull(value);
        }
        else if(key == "SwapFree:")
        {
            parsed = true;
            ifs >> value;
            swapFree = std::stoull(value);
        }
    }
    if(!parsed)
    {
        throw MemoryParseError("MemoryInfo::parseMemInfoFile(): meminfo file is incorrect - parsed nothing!");
    }
}
