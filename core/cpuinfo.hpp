#pragma once
#include "parsers/cpu_parsers.hpp"

/*
    WARNING: cpuInfoCount and size of load vector is always core count + 1
        so 0 record is total cpu information,
        and next is info by cores.
        So if you have 8 cores, cpuInfoCount() will return 9.
    stat is the stat file.
*/
class CPUInfo
{
public:
    typedef std::shared_ptr<CPUInfo> pointer;
    static CPUInfo::pointer fromStatFile(const std::string& statFile = "/proc/stat");
    inline size_t cpuCount() const { return parser->getCpuCount(); }
    inline float getLoad(size_t core) const {return parser->getLoad(core);}
    uint64_t getTotalWorkTime() const { return parser->getTotalWorkTime(); }
    void update();
private:
    CPUInfo() {}
    void parse();
    CPUParser::pointer parser;
};


