#pragma once
#include <applicationnamespace.hpp>
#include "core/cpuinfo.hpp"
#include "core/meminfo.hpp"
#include "core/netinfo.hpp"
#include "core/hddinfo.hpp"
#include "core/procinfo.hpp"
#include <thread>
#include <mutex>

/*
    This class manages signletons classes CPUInfo, MemInfo, NetInfo creation, updating and access to.
    Designed to use in multithread context.
    SET UPDATE INTERVALS TO 0 TO SWITCH OFF UPDATES(daemon will awake every second for check)
*/
class InfoManager
{
public:
    typedef std::shared_ptr<InfoManager> pointer;
    typedef ProcessesParser::PID PID;
    typedef std::unordered_map<PID, ProcessActivities> ActivitiesMap;
    InfoManager(bool initCpu = true, bool initMem = true, bool initNet = true, bool initHdd = true, bool initProc = true);
    ~InfoManager();
    void setCpuUpdateInterval(int interval);
    void setMemUpdateInterval(int interval);
    void setNetUpdateInterval(int interval);
    void setHddUpdateInterval(int interval);
    void setProcUpdateInterval(int interval);

    inline bool isCpuInfoInitialized() const { return cpuInfoInitialized; }
    inline bool isMemInfoInitialized() const { return memInfoInitialized; }
    inline bool isNetInfoInitialized() const { return netInfoInitialized; }
    inline bool isHddInfoInitialized() const { return hddInfoInitialized; }
    inline bool isProcInfoInitialized() const { return procInfoInitialized; }

    //cpu
    size_t cpuCount() const;
    float getLoad(size_t core) const;
    uint64_t getTotalWorkTime() const;

    //mem
    float getMemoryUsage() const;
    float getSwapUsage() const;
    uint64_t getMemoryTotal() const;
    uint64_t getMemoryBusy() const;
    uint64_t getMemoryAvailable() const;
    uint64_t getSwapTotal() const;
    uint64_t getSwapBusy() const;
    uint64_t getSwapAvailable() const;
    uint64_t getMemoryPageSize() const;

    // net
    uint64_t getIncome() const;
    uint64_t getOutcome() const;

    // hdd
    const HddParser::DeviceInfoMap& getDeviceInfoMap() const;
    const DeviceActivity& getDeviceActivity() const;

    // proc
    const ProcInfo::LinuxProcinfoMap& getLinuxProcessInfoMap() const;
    const ProcessActivities& getProcessActivities(PID pid);
    const ActivitiesMap& getActivitiesMap() const;

private:
    void cpuUpdaterDaemon();
    void memUpdaterDaemon();
    void netUpdaterDaemon();
    void hddUpdaterDaemon();
    void procUpdaterDaemon();

    void updateProcessesActivities();
    void updateLinuxProcessesActivities();

    ActivitiesMap activitiesMap;
    // needed for updating activities map
    ProcInfo::LinuxProcinfoMap lastLinuxMap;
    uint64_t lastCpuTotalWorkTime;

    std::thread cpuUpdaterDaemonThread;
    std::thread memUpdaterDaemonThread;
    std::thread netUpdaterDaemonThread;
    std::thread hddUpdaterDaemonThread;
    std::thread procUpdaterDaemonThread;
    mutable std::mutex cpuInfoMtx;
    mutable std::mutex memInfoMtx;
    mutable std::mutex netInfoMtx;
    mutable std::mutex hddInfoMtx;
    mutable std::mutex procInfoMtx;
    CPUInfo::pointer cpuInfo;
    MemInfo::pointer memInfo;
    NetInfo::pointer netInfo;
    HddInfo::pointer hddInfo;
    ProcInfo::pointer procInfo;
    int cpuUpdateInterval;
    int memUpdateInterval;
    int netUpdateInterval;
    int hddUpdateInterval;
    int procUpdateInterval;
    bool terminated;
    bool cpuInfoInitialized;
    bool memInfoInitialized;
    bool netInfoInitialized;
    bool hddInfoInitialized;
    bool procInfoInitialized;
};
