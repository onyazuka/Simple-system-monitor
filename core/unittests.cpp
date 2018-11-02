#include <thread>
#include <gtest/gtest.h>
#include "meminfo.hpp"
#include "cpuinfo.hpp"
#include "netinfo.hpp"
#include "parseerror.hpp"

TEST(MemoryParseTest, MemoryTest)
{
    // throw: not existing meminfo file
    EXPECT_THROW({
                     MemInfo meminfo("/proc/meinfo");
                 }, MemoryParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                     MemInfo meminfo("/proc/stat");
                 }, MemoryParseError);

    // ok
    EXPECT_NO_THROW({
                        MemInfo meminfo;
                        MemInfo meminfo2("/proc/meminfo");
                    });

    // same tests for static creation

    // throw: not existing meminfo file
    EXPECT_THROW({
                     MemInfo meminfo = MemInfo::fromMemInfoFile("/proc/meinfo");
                 }, MemoryParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                      MemInfo meminfo = MemInfo::fromMemInfoFile("/proc/stat");
                 }, MemoryParseError);

    // ok
    EXPECT_NO_THROW({
                         MemInfo meminfo = MemInfo::fromMemInfoFile();
                         MemInfo meminfo2 = MemInfo::fromMemInfoFile("/proc/meminfo");
                    });
    // ok: update test
    EXPECT_NO_THROW({
                         MemInfo meminfo = MemInfo::fromMemInfoFile();
                         meminfo.update();
                         meminfo.update();
                    });

    // ok: some basic checks
    MemInfo meminfo = MemInfo::fromMemInfoFile();
    EXPECT_TRUE(meminfo.memTotal != 0);
    EXPECT_TRUE(meminfo.memTotal >= meminfo.memAvailable);
    EXPECT_TRUE(meminfo.swapTotal >= meminfo.swapFree);
}

TEST(CPUParseTest, CPUTest)
{
    // throw: not existing meminfo file
    EXPECT_THROW({
                     CPUInfo cpuinfo("/proc/sta");
                 }, CPUParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                     CPUInfo cpuinfo("/proc/meminfo");
                 }, CPUParseError);

    // ok
    EXPECT_NO_THROW({
                        CPUInfo cpuinfo;
                        CPUInfo cpuinfo2("/proc/stat");
                    });

    // same tests for static creation

    // throw: not existing meminfo file
    EXPECT_THROW({
                     CPUInfo cpuinfo = CPUInfo::fromStatFile("/proc/sta");
                 }, CPUParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                     CPUInfo cpuinfo = CPUInfo::fromStatFile("/proc/meminfo");
                 }, CPUParseError);

    // ok
    EXPECT_NO_THROW({
                        CPUInfo cpuinfo = CPUInfo::fromStatFile();
                        CPUInfo cpuinfo2 = CPUInfo::fromStatFile("/proc/stat");
                    });

    // ok: doing some updates
    EXPECT_NO_THROW({
                        CPUInfo cpuinfo2 = CPUInfo::fromStatFile("/proc/stat");
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        cpuinfo2.update();
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        cpuinfo2.update();
                    });

    // ok: cpu count is not 0
    CPUInfo cpuinfo;
    EXPECT_TRUE(cpuinfo.cpuInfoCount() != 0);
}

TEST(NetworkParseTest, NetworkTest)
{
    // throw: not existing meminfo file
    EXPECT_THROW({
                     NetInfo netinfo("/proc/net/netstar");
                 }, NetworkParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                     NetInfo netinfo("/proc/stat");
                 }, NetworkParseError);

    // ok
    EXPECT_NO_THROW({
                        NetInfo netinfo;
                        NetInfo netinfo2("/proc/net/netstat");
                    });

    // same tests for static creation

    // throw: not existing meminfo file
    EXPECT_THROW({
                     NetInfo netinfo = NetInfo::fromNetInfoFile("/proc/meinfo");
                 }, NetworkParseError);

    // throw: file is existing, but incorrect
    EXPECT_THROW({
                      NetInfo netinfo = NetInfo::fromNetInfoFile("/proc/stat");
                 }, NetworkParseError);

    // ok
    EXPECT_NO_THROW({
                         NetInfo netinfo = NetInfo::fromNetInfoFile();
                         NetInfo netinfo2 = NetInfo::fromNetInfoFile("/proc/net/netstat");
                    });

    // ok: testing update
    EXPECT_NO_THROW({
                         NetInfo netinfo = NetInfo::fromNetInfoFile();
                         netinfo.update();
                         netinfo.update();
                    });


    // ok: some basic tests
    NetInfo netinfo;
    NetInfo::Keys keys = netinfo.keys();
    NetInfo::Values values = netinfo.values();
    EXPECT_TRUE(keys.size() != 0);
    EXPECT_TRUE(values.size() != 0);
}
