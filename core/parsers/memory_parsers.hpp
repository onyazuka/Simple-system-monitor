#pragma once
#include "parser.hpp"

#ifdef __linux__
#include <unistd.h>
#endif

struct MemoryInfo
{
    uint64_t memoryTotal;
    uint64_t memoryFree;
    uint64_t swapTotal;
    uint64_t swapFree;
};

class MemoryParser : public Parser
{
public:
    typedef std::unique_ptr<MemoryParser> pointer;
    virtual void parse() = 0;
    virtual void update() = 0;
    virtual MemoryInfo getMemoryInfo() const = 0;
    virtual uint64_t getPageSize() const = 0;
};

class MeminfoMemoryParser : public MemoryParser
{
public:
    typedef std::unique_ptr<MeminfoMemoryParser> pointer;
    MeminfoMemoryParser(const std::string& _meminfoFile);
    void parse() final;
    void update() final;
    inline MemoryInfo getMemoryInfo() const final { return memoryInfo; }
    inline uint64_t getPageSize() const { return pageSize; }
    inline const std::string& getMeminfoFileName() const { return meminfoFile; }
private:
    std::string meminfoFile;
    MemoryInfo memoryInfo;
    uint64_t pageSize;  // in bytes
};
