#pragma once
#include "parsers/memory_parsers.hpp"

/*
    Main memory info
*/
struct MemInfo
{
    typedef std::shared_ptr<MemInfo> pointer;
    static MemInfo::pointer fromMemInfoFile(const std::string& infoFile = "/proc/meminfo");
    void update();
    inline float getMemoryUsage() const {return (float)(memTotal - memAvailable) / memTotal * 100;}
    inline float getSwapUsage() const {return (float)(swapTotal - swapFree) / swapTotal * 100;}
    inline uint64_t getMemoryTotal() const { return memTotal; }
    inline uint64_t getMemoryBusy() const { return memTotal - memAvailable; }
    inline uint64_t getMemoryAvailable() const { return memAvailable; }
    inline uint64_t getSwapTotal() const { return swapTotal; }
    inline uint64_t getSwapAvailable() const { return swapFree; }
    inline uint64_t getSwapBusy() const { return swapTotal - swapFree; }
    inline uint64_t getPageSize() const { return parser->getPageSize(); }
private:
    MemInfo() {}
    void parse();

    MemoryParser::pointer parser;
    uint64_t memTotal;
    uint64_t memAvailable;
    uint64_t swapTotal;
    uint64_t swapFree;
};
