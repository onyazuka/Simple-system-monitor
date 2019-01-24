#include "meminfo.hpp"

MemInfo::pointer MemInfo::fromMemInfoFile(const std::string& infoFile)
{
    MemInfo::pointer newMemInfo(new MemInfo());
    newMemInfo->parser = MeminfoMemoryParser::pointer(new MeminfoMemoryParser(infoFile));
    newMemInfo->parse();
    return newMemInfo;
}

void MemInfo::update()
{
    parser->update();
    MemoryInfo meminfo = parser->getMemoryInfo();
    memTotal = meminfo.memoryTotal;
    memAvailable = meminfo.memoryTotal - meminfo.memoryFree;
    swapTotal = meminfo.swapTotal;
    swapFree = meminfo.swapFree;
}

void MemInfo::parse()
{
    update();
}
