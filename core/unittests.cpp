#include <thread>
#include <gtest/gtest.h>
#include "meminfo.hpp"
#include "cpuinfo.hpp"
#include "netinfo.hpp"
#include "hddinfo.hpp"
#include "procinfo.hpp"

TEST(MemoryParseTest, MemoryTest)
{

    // throw: not existing meminfo file
    EXPECT_THROW({
                     MemInfo::pointer meminfo = MemInfo::fromMemInfoFile("/proc/meinfo");
                 }, MemoryParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                      MemInfo::pointer meminfo = MemInfo::fromMemInfoFile("/proc/stat");
                 }, MemoryParseError);

    // ok
    EXPECT_NO_THROW({
                         MemInfo::pointer meminfo = MemInfo::fromMemInfoFile();
                         MemInfo::pointer meminfo2 = MemInfo::fromMemInfoFile("/proc/meminfo");
                    });
    // ok: update test
    EXPECT_NO_THROW({
                         MemInfo::pointer meminfo = MemInfo::fromMemInfoFile();
                         meminfo->update();
                         meminfo->update();
                    });

    // ok: some basic checks
    MemInfo::pointer meminfo = MemInfo::fromMemInfoFile();
    EXPECT_TRUE(meminfo->getMemoryTotal() != 0);
    EXPECT_TRUE(meminfo->getMemoryTotal() >= meminfo->getMemoryAvailable());
    EXPECT_TRUE(meminfo->getSwapTotal() >= meminfo->getSwapAvailable());
}

TEST(CPUParseTest, CPUTest)
{

    // throw: not existing meminfo file
    EXPECT_THROW({
                     CPUInfo::pointer cpuinfo = CPUInfo::fromStatFile("/proc/sta");
                 }, CPUParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                     CPUInfo::pointer cpuinfo = CPUInfo::fromStatFile("/proc/meminfo");
                 }, CPUParseError);

    // ok
    EXPECT_NO_THROW({
                        CPUInfo::pointer cpuinfo = CPUInfo::fromStatFile();
                        CPUInfo::pointer cpuinfo2 = CPUInfo::fromStatFile("/proc/stat");
                    });

    // ok: doing some updates
    EXPECT_NO_THROW({
                        CPUInfo::pointer cpuinfo2 = CPUInfo::fromStatFile("/proc/stat");
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        cpuinfo2->update();
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        cpuinfo2->update();
                    });

    // ok: cpu count is not 0
    CPUInfo::pointer cpuinfo = CPUInfo::fromStatFile();
    EXPECT_TRUE(cpuinfo->cpuCount() != 0);
}

TEST(NetworkParseTest, NetworkTest)
{

    // throw: not existing meminfo file
    EXPECT_THROW({
                     NetInfo::pointer netinfo = NetInfo::fromNetInfoFile("/proc/meinfo");
                 }, NetworkParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                      NetInfo::pointer netinfo = NetInfo::fromNetInfoFile("/proc/stat");
                 }, NetworkParseError);

    // ok
    EXPECT_NO_THROW({
                         NetInfo::pointer netinfo = NetInfo::fromNetInfoFile();
                         NetInfo::pointer netinfo2 = NetInfo::fromNetInfoFile("/proc/net/netstat");
                    });

    // ok: testing update
    EXPECT_NO_THROW({
                         NetInfo::pointer netinfo = NetInfo::fromNetInfoFile();
                         netinfo->update();
                         netinfo->update();
                    });
}

TEST(HddParseTest, HddTest)
{
    HddInfo::pointer hddinfo = HddInfo::makeInLinuxWay();
    hddinfo->update();
    auto dmap = hddinfo->getDeviceInfoMap();
    EXPECT_NE(dmap.size(), 0);
    EXPECT_NE((dmap.begin())->second.totalSpace, 0);
    DeviceActivity da = hddinfo->getDeviceActivity();
    EXPECT_NE(da.bytesRead, 0);
    EXPECT_NE(da.bytesWrite, 0);
}

TEST(ProcParseTest, ProcTest)
{
    ProcInfo::pointer procInfo;
    EXPECT_NO_THROW
    ({
         procInfo = ProcInfo::makeInLinuxWay();
    });
    procInfo->update();
    EXPECT_NE(procInfo->getLinuxProcessInfoMap().size(), 0);
    // pid 1 - systemd
    auto info = procInfo->getLinuxProcessInfoMap();
    EXPECT_EQ(info[1].procinfo.pid, 1);
    EXPECT_NE(info[1].procinfo.utime, 0);
    EXPECT_NE(info[1].procinfo.stime, 0);
}
