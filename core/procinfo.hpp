#pragma once
#include "parsers/processes_parsers.hpp"

// means activities relative last time, not absolute
struct ProcessActivities
{
    typedef ProcessesParser::PID PID;
    // we are doubling key so we can search by it in map and we can get vector of values and sort them in any way
    PID pid;
    std::string name;
    float cpuUsage;         // percents
    float memUsage;         // percents
    float memUsageBytes;
    uint64_t ioRead;
    uint64_t ioWrite;
    // planning to add uid and gid
    uint64_t ioReadNow;
    uint64_t ioWriteNow;
    PID ppid;
};

class ProcInfo
{
public:
    typedef std::shared_ptr<ProcInfo> pointer;
    typedef ProcessesParser::LinuxProcinfoMap LinuxProcinfoMap;
    static ProcInfo::pointer makeInLinuxWay();
    void update();
    inline const LinuxProcinfoMap& getLinuxProcessInfoMap() const { return parser->getLinuxProcessInfoMap(); }
private:
    ProcInfo() {}
    void parse();

    ProcessesParser::pointer parser;
};
