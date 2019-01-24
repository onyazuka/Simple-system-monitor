#include "cpuinfo.hpp"


CPUInfo::pointer CPUInfo::fromStatFile(const std::string& statFile)
{
    CPUInfo::pointer info(new CPUInfo());
    info->parser = StatFileCPUParser::pointer(new StatFileCPUParser(statFile));
    info->parse();
    return info;
}

void CPUInfo::update()
{
    parser->update();
}


void CPUInfo::parse()
{
    parser->parse();
}



