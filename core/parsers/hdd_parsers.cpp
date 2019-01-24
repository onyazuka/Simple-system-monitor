#include "hdd_parsers.hpp"

#ifdef __linux__

const std::vector<std::string> LinuxHddParser::mountsFiles {"/proc/mounts"};
const std::vector<std::string> LinuxHddParser::logicBlockSizeFiles{"/sys/block/sda/queue/logical_block_size"};
const std::vector<std::string> LinuxHddParser::diskstatsFiles{"/proc/diskstats"};

LinuxHddParser::LinuxHddParser()
{
    initFiles();
    getLogicalBlockSize();
    setMountedsList();
}

void LinuxHddParser::parse()
{
    setMountedsList();
    getDevicesSpaceInfo();
    getDevicesActivity();
}

void LinuxHddParser::update()
{
    parse();
}

/*
    tries to find all needed for working files
    if it can not, throws
*/
void LinuxHddParser::initFiles()
{
    for(size_t i = 0; i < logicBlockSizeFiles.size(); ++i)
    {
        std::ifstream ifs{logicBlockSizeFiles[i]};
        if(!ifs) continue;
        logicBlockFile = logicBlockSizeFiles[i];
    }
    for(size_t i = 0; i < mountsFiles.size(); ++i)
    {
        std::ifstream ifs{mountsFiles[i]};
        if(!ifs) continue;
        mountsFile = mountsFiles[i];
    }
    for(size_t i = 0; i < diskstatsFiles.size(); ++i)
    {
        std::ifstream ifs{diskstatsFiles[i]};
        if(!ifs) continue;
        diskstatsFile = diskstatsFiles[i];
    }
    if(mountsFile.empty() || logicBlockFile.empty() || diskstatsFile.empty())
    {
        throw HddParseError("Linux Hdd Parser: has not been initted");
    }
}

 void LinuxHddParser::setMountedsList()
{
    std::ifstream ifs{mountsFile};
    if(!ifs)
    {
        throw HddParseError("Linux Hdd Parser: can not parse mounted devices list");
    }
    // nothing changed
    if(!lastReadMountsFile.empty() && areMountEqualWithLast(mountsFile)) return;
    // else parsing
    deviceInfoMap.clear();
    std::string curStr;
    while(ifs)
    {
        ifs >> curStr;
        if(curStr.compare(0, 7, "/dev/sd") == 0)
        {
            std::string path;
            ifs >> path;
            DeviceInfo deviceInfo;
            // filling this in getDevicesSpaceInfo()
            deviceInfo.freeSpace = -1;
            deviceInfo.physicalBlockSize = -1;
            deviceInfo.totalSpace = -1;
            deviceInfo.devicePath = std::move(path);
            deviceInfoMap[curStr] = deviceInfo;
        }
    }
}

 void LinuxHddParser::getDevicesSpaceInfo()
 {
     for(auto& device : deviceInfoMap)
     {
         struct statvfs deviceStats;
         statvfs(device.second.devicePath.data(), &deviceStats);
         DeviceInfo curDeviceInfo;
         curDeviceInfo.physicalBlockSize = deviceStats.f_bsize;
         curDeviceInfo.totalSpace = deviceStats.f_blocks * curDeviceInfo.physicalBlockSize;
         curDeviceInfo.freeSpace = deviceStats.f_bfree * curDeviceInfo.physicalBlockSize;
         curDeviceInfo.devicePath = std::move(device.second.devicePath);
         device.second = std::move(curDeviceInfo);
     }
 }

 void LinuxHddParser::getDevicesActivity()
 {
     std::ifstream ifs(diskstatsFile);
     if(!ifs)
     {
         throw HddParseError("Linux Hdd Parser: error parsing devices activities");
     }
     std::string curStr;
     uint64_t readsCompleted, readsMerged, sectorsRead, timeReading, writesCompleted, writesMerged, sectorsWritten, timeWriting;
     deviceActivity.bytesRead = 0;
     deviceActivity.bytesWrite = 0;
     while(ifs)
     {
         ifs >> curStr;
         if(curStr.compare(0, 2, "sd") == 0)
         {
             ifs >> readsCompleted >> readsMerged >> sectorsRead >> timeReading >> writesCompleted >> writesMerged >> sectorsWritten >> timeWriting;
             if(!ifs)
             {
                 throw HddParseError("Linux Hdd Parser: error parsing devices activities");
             }
             deviceActivity.bytesRead += sectorsRead * logicalBlockSize;
             deviceActivity.bytesWrite += sectorsWritten * logicalBlockSize;
         }
     }
 }

void LinuxHddParser::getLogicalBlockSize()
{
    std::ifstream ifs{logicBlockFile};
    if(!ifs)
    {
        throw HddParseError("Linux Hdd Parser: error parsing logical block size");
    }
    ifs >> logicalBlockSize;
    if(logicalBlockSize == 0)
    {
        throw HddParseError("Linux Hdd Parser: error parsing logical block size");
    }
}

/*
    except parsing mounts file each time, we are checking if last reading equal to current reading
    (optimization)
*/
bool LinuxHddParser::areMountEqualWithLast(const std::string& mountFile)
{
    std::ifstream ifs{mountFile};
    if(!ifs)
    {
        throw HddParseError("Linux Hdd Parser: can not parse mounted devices list");
    }
    std::string thisReadMountsFile;
    ifs.seekg(0, std::ios_base::end);
    thisReadMountsFile.resize(ifs.tellg());
    ifs.seekg(0, std::ios_base::beg);
    ifs.read(&thisReadMountsFile[0], thisReadMountsFile.size());
    if(memcmp(&thisReadMountsFile[0], &lastReadMountsFile[0], thisReadMountsFile.size()) == 0)
    {
        return true;
    }
    return false;
}

#endif


