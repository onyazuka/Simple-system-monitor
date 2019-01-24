#include "netinfo.hpp"

NetInfo::pointer NetInfo::fromNetInfoFile(const std::string &infoFile)
{
    NetInfo::pointer netinfo(new NetInfo());
    netinfo->parser = NetstatNetworkParser::pointer(new NetstatNetworkParser(infoFile));
    netinfo->parse();
    return netinfo;
}

void NetInfo::update()
{
    parser->update();
}

void NetInfo::parse()
{
    parser->parse();
}


