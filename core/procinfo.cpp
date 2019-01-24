#include "procinfo.hpp"

ProcInfo::pointer ProcInfo::makeInLinuxWay()
{
    ProcInfo::pointer info(new ProcInfo());
    info->parser = LinuxProcessesParser::pointer(new LinuxProcessesParser());
    info->parse();
    return info;
}

void ProcInfo::update()
{
    parser->update();
}

void ProcInfo::parse()
{
    parser->parse();
}
