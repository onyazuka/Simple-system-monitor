#include "memory_parsers.hpp"

// throws on error
MeminfoMemoryParser::MeminfoMemoryParser(const std::string& _meminfoFile)
    : meminfoFile(_meminfoFile)
{
    pageSize = sysconf(_SC_PAGESIZE);
}

void MeminfoMemoryParser::parse()
{
    std::ifstream ifs{meminfoFile};
    if(!ifs)
    {
        throw MemoryParseError("MeminfoMemoryParser::parse(): can not open file");
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
            memoryInfo.memoryTotal = std::stoull(value);
        }
        else if (key == "MemAvailable:")
        {
            parsed = true;
            ifs >> value;
            memoryInfo.memoryFree = memoryInfo.memoryTotal - std::stoull(value);
        }
        else if(key == "SwapTotal:")
        {
            parsed = true;
            ifs >> value;
            memoryInfo.swapTotal = std::stoull(value);
        }
        else if(key == "SwapFree:")
        {
            parsed = true;
            ifs >> value;
            memoryInfo.swapFree = std::stoull(value);
        }
    }
    if(!parsed)
    {
        throw MemoryParseError("MeminfoMemoryParser::parse(): meminfo file is incorrect - parsed nothing!");
    }
}

void MeminfoMemoryParser::update()
{
    parse();
}
