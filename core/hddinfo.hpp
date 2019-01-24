#pragma once
#include "parsers/hdd_parsers.hpp"

class HddInfo
{
public:
    typedef std::shared_ptr<HddInfo> pointer;
    typedef std::map<std::string, DeviceInfo> DeviceInfoMap;
    static HddInfo::pointer makeInLinuxWay();
    void update();
    inline const HddParser::DeviceInfoMap& getDeviceInfoMap() const { return parser->getDeviceInfoMap(); }
    inline const DeviceActivity& getDeviceActivity() const { return parser->getDeviceActivity(); }
private:
    HddInfo() {}
    void parse();

    HddParser::pointer parser;
};
