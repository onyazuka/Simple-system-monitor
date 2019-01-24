#include "hddinfo.hpp"

HddInfo::pointer HddInfo::makeInLinuxWay()
{
    HddInfo::pointer info(new HddInfo());
    info->parser = LinuxHddParser::pointer(new LinuxHddParser());
    info->parse();
    return info;
}

void HddInfo::update()
{
    parser->update();
}

void HddInfo::parse()
{
    parser->parse();
}
