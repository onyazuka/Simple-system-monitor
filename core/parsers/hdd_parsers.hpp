#pragma once
#include "parser.hpp"
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <string.h>

#ifdef __linux__
#include <sys/statvfs.h>
#endif

struct DeviceInfo
{
    std::string devicePath;
    uint64_t physicalBlockSize;
    uint64_t totalSpace;
    uint64_t freeSpace;
};

struct DeviceActivity
{
    uint64_t bytesRead;
    uint64_t bytesWrite;
};

class HddParser : public Parser
{
public:
    typedef std::unique_ptr<HddParser> pointer;
    typedef std::map<std::string, DeviceInfo> DeviceInfoMap;
    virtual void parse() = 0;
    virtual void update() = 0;
    virtual const DeviceInfoMap& getDeviceInfoMap() const = 0;
    virtual const DeviceActivity& getDeviceActivity() const = 0;
};

#ifdef __linux__
class LinuxHddParser : public HddParser
{
public:
    typedef std::unique_ptr<LinuxHddParser> pointer;
    typedef std::vector<std::string> MountedList;
    LinuxHddParser();
    void parse() final;
    void update() final;
    inline const DeviceInfoMap& getDeviceInfoMap() const final { return deviceInfoMap; }
    inline const DeviceActivity& getDeviceActivity() const final { return deviceActivity; }
private:
    void initFiles();
    void setMountedsList();
    void getDevicesSpaceInfo();
    void getDevicesActivity();
    void getLogicalBlockSize();

    bool areMountEqualWithLast(const std::string& mountFile);

    DeviceInfoMap deviceInfoMap;
    DeviceActivity deviceActivity;
    int logicalBlockSize;
    std::string lastReadMountsFile;

    std::string mountsFile;
    std::string logicBlockFile;
    std::string diskstatsFile;

    static const std::vector<std::string> mountsFiles;
    static const std::vector<std::string> logicBlockSizeFiles;
    static const std::vector<std::string> diskstatsFiles;
};
#endif
