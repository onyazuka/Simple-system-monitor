#include "infomanager.hpp"

/*
    Creates instances of all *info types.
    Those bool init* flags was added half for testing purpose, half for possibility to enable/disable widgets in settings.
*/
InfoManager::InfoManager(bool initCpu, bool initMem, bool initNet, bool initHdd, bool initProc)
    : cpuUpdateInterval{0}, memUpdateInterval{0}, netUpdateInterval{0},
      hddUpdateInterval{0}, procUpdateInterval{0}, terminated{false},
      cpuInfoInitialized{false}, memInfoInitialized{false}, netInfoInitialized{false},
      hddInfoInitialized{false}, procInfoInitialized{false}
{
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    std::lock_guard<std::mutex> lcknet(netInfoMtx);
    std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
    std::lock_guard<std::mutex> lckpro(procInfoMtx);
    if(initCpu)
    {
        try
        {
            cpuInfo = std::move(CPUInfo::fromStatFile());
            cpuInfoInitialized = true;

        }
        catch(CPUParseError&) {}
    }
    if(initMem)
    {
        try
        {
            memInfo = std::move(MemInfo::fromMemInfoFile());
            memInfoInitialized = true;
        }
        catch (MemoryParseError&) {}
    }
    if(initNet)
    {
        try
        {
            netInfo = std::move(NetInfo::fromNetInfoFile());
            netInfoInitialized = true;
        }
        catch(NetworkParseError&) {}
    }
    if(initHdd)
    {
        try
        {
            hddInfo = std::move(HddInfo::makeInLinuxWay());
            hddInfoInitialized = true;
        }
        catch (HddParseError&) {}
    }

    if(initProc)
    {
        try
        {
            procInfo = std::move(ProcInfo::makeInLinuxWay());
            procInfoInitialized = true;
        }
        catch (ProcessesParseError&) {}
    }

    if(cpuInfoInitialized)
    {
         cpuUpdaterDaemonThread = std::thread(&InfoManager::cpuUpdaterDaemon, this);
    }
    if(memInfoInitialized)
    {
        memUpdaterDaemonThread = std::thread(&InfoManager::memUpdaterDaemon, this);
    }
    if(netInfoInitialized)
    {
        netUpdaterDaemonThread = std::thread(&InfoManager::netUpdaterDaemon, this);
    }
    if(hddInfoInitialized)
    {
        hddUpdaterDaemonThread = std::thread(&InfoManager::hddUpdaterDaemon, this);
    }
    if(procInfoInitialized)
    {
        procUpdaterDaemonThread = std::thread(&InfoManager::procUpdaterDaemon, this);
    }
}

InfoManager::~InfoManager()
{
    terminated = true;
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    std::lock_guard<std::mutex> lcknet(netInfoMtx);
    std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
    std::lock_guard<std::mutex> lckpro(procInfoMtx);
    if(cpuInfoInitialized)
    {
        cpuUpdaterDaemonThread.join();
    }
    if(memInfoInitialized)
    {
        memUpdaterDaemonThread.join();
    }
    if(netInfoInitialized)
    {
        netUpdaterDaemonThread.join();
    }
    if(hddInfoInitialized)
    {
        hddUpdaterDaemonThread.join();
    }
    if(procInfoInitialized)
    {
        procUpdaterDaemonThread.join();
    }
}

// SET UPDATE INTERVALS TO 0 TO SWITCH OFF UPDATES(daemon will awake every second for check)
void InfoManager::setCpuUpdateInterval(int interval)
{
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    cpuUpdateInterval = interval;
}

void InfoManager::setMemUpdateInterval(int interval)
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    memUpdateInterval = interval;
}

void InfoManager::setNetUpdateInterval(int interval)
{
    std::lock_guard<std::mutex> lcknet(netInfoMtx);
    netUpdateInterval = interval;
}

void InfoManager::setHddUpdateInterval(int interval)
{
    std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
    hddUpdateInterval = interval;
}

void InfoManager::setProcUpdateInterval(int interval)
{
    std::lock_guard<std::mutex> lckpro(procInfoMtx);
    procUpdateInterval = interval;
}

void InfoManager::cpuUpdaterDaemon()
{
    while(true)
    {
        if(terminated)
        {
            break;
        }
        if(!(cpuInfo == nullptr) && !(cpuUpdateInterval == 0))
        {
            std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
            cpuInfo->update();
        }
        if(cpuUpdateInterval != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(cpuUpdateInterval));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void InfoManager::memUpdaterDaemon()
{
    while(true)
    {
        if(terminated)
        {
            break;
        }
        if(!(memInfo == nullptr) && !(memUpdateInterval == 0))
        {
            std::lock_guard<std::mutex> lckmem(memInfoMtx);
            memInfo->update();
        }
        if(memUpdateInterval != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(memUpdateInterval));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void InfoManager::netUpdaterDaemon()
{
    while(true)
    {
        if(terminated)
        {
            break;
        }
        if(!(netInfo == nullptr) && !(netUpdateInterval == 0))
        {
            std::lock_guard<std::mutex> lcknet(netInfoMtx);
            netInfo->update();
        }
        if(netUpdateInterval != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(netUpdateInterval));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void InfoManager::hddUpdaterDaemon()
{
    while(true)
    {
        if(terminated)
        {
            break;
        }
        if(!(hddInfo == nullptr) && !(hddUpdateInterval == 0))
        {
            std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
            hddInfo->update();
        }
        if(hddUpdateInterval != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(hddUpdateInterval));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void InfoManager::procUpdaterDaemon()
{
    while(true)
    {
        if(terminated)
        {
            break;
        }
        if(!(procInfo == nullptr) && !(procUpdateInterval == 0))
        {
            std::lock_guard<std::mutex> lckpro(procInfoMtx);
            procInfo->update();
            updateProcessesActivities();
        }
        if(procUpdateInterval != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(procUpdateInterval));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

/*
    WARNING! Should be called only by procUpdaterDaemon.
*/
void InfoManager::updateProcessesActivities()
{
    try
    {
        activitiesMap.clear();
        procInfo->getLinuxProcessInfoMap();
        updateLinuxProcessesActivities();
        if(cpuInfoInitialized)
        {
            lastCpuTotalWorkTime = cpuInfo->getTotalWorkTime();
        }
        lastLinuxMap = procInfo->getLinuxProcessInfoMap();
    }
    // catch is prolonged
    catch(NotImplementedError& nie)
    {
        throw(NotImplementedError(nie));
    }
}

/*
    WARNING! Should be called only by updateProcessesActivities.
*/
void InfoManager::updateLinuxProcessesActivities()
{

    // item.first is PID, item.second is LinuxProcessInfo
    for(auto item: procInfo->getLinuxProcessInfoMap())
    {
        PID pid = item.first;
        LinuxProcessInfo pinfo = item.second;
        ProcessActivities thisProcessActivities;
        thisProcessActivities.cpuUsage = 0;
        thisProcessActivities.memUsage = 0;
        thisProcessActivities.pid = pid;
        thisProcessActivities.name = pinfo.procinfo.procName;
        if(memInfoInitialized)
        {
            // page size is in bytes, rss is in kilobytes
            thisProcessActivities.memUsage = (float)pinfo.procinfo.rss * (memInfo->getPageSize() / 1024) / (memInfo->getMemoryTotal());
            thisProcessActivities.memUsageBytes = (float)pinfo.procinfo.rss * memInfo->getPageSize();
        }
        thisProcessActivities.ioRead = pinfo.ioinfo.read_bytes;
        thisProcessActivities.ioWrite = pinfo.ioinfo.write_bytes;
        if(cpuInfoInitialized)
        {
            try
            {
                LinuxProcessInfo lastpinfo = lastLinuxMap[pid];
                thisProcessActivities.ioReadNow = pinfo.ioinfo.read_bytes - lastpinfo.ioinfo.read_bytes;
                thisProcessActivities.ioWriteNow = pinfo.ioinfo.write_bytes - lastpinfo.ioinfo.write_bytes;
                thisProcessActivities.ppid = pinfo.procinfo.ppid;
                // 0 division
                if((cpuInfo->getTotalWorkTime() - lastCpuTotalWorkTime) == 0)
                {
                    thisProcessActivities.cpuUsage = 0;
                }
                else
                {
                    // cpuCount() - 1 is important
                    thisProcessActivities.cpuUsage = (float)((pinfo.procinfo.utime + pinfo.procinfo.stime) -
                            (lastpinfo.procinfo.utime + lastpinfo.procinfo.stime)) /
                            (cpuInfo->getTotalWorkTime() - lastCpuTotalWorkTime) / (cpuInfo->cpuCount() - 1);
                }

            }
            // process just started to work
            catch(std::out_of_range)
            {
                thisProcessActivities.cpuUsage = (float)(pinfo.procinfo.utime + pinfo.procinfo.stime) /
                        (cpuInfo->getTotalWorkTime() - lastCpuTotalWorkTime) / (cpuInfo->cpuCount() - 1);
            }
        }
        activitiesMap[pid] = std::move(thisProcessActivities);
    }
}

size_t InfoManager::cpuCount() const
{
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    return cpuInfo->cpuCount();
}

float InfoManager::getLoad(size_t core) const
{
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    return cpuInfo->getLoad(core);
}

uint64_t InfoManager::getTotalWorkTime() const
{
    std::lock_guard<std::mutex> lckcpu(cpuInfoMtx);
    return cpuInfo->getTotalWorkTime();
}

//mem
float InfoManager::getMemoryUsage() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getMemoryUsage();
}

float InfoManager::getSwapUsage() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getSwapUsage();
}

uint64_t InfoManager::getMemoryTotal() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getMemoryTotal();
}

uint64_t InfoManager::getMemoryBusy() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getMemoryBusy();
}

uint64_t InfoManager::getMemoryAvailable() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getMemoryAvailable();
}

uint64_t InfoManager::getSwapTotal() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getSwapTotal();
}

uint64_t InfoManager::getSwapBusy() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getSwapBusy();
}

uint64_t InfoManager::getSwapAvailable() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getSwapAvailable();
}

uint64_t InfoManager::getMemoryPageSize() const
{
    std::lock_guard<std::mutex> lckmem(memInfoMtx);
    return memInfo->getPageSize();
}

// net
uint64_t InfoManager::getIncome() const
{
    std::lock_guard<std::mutex> lcknet(netInfoMtx);
    return netInfo->getIncome();
}

uint64_t InfoManager::getOutcome() const
{
    std::lock_guard<std::mutex> lcknet(netInfoMtx);
    return netInfo->getOutcome();
}

// hdd
const HddParser::DeviceInfoMap& InfoManager::getDeviceInfoMap() const
{
    std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
    return hddInfo->getDeviceInfoMap();
}

const DeviceActivity& InfoManager::getDeviceActivity() const
{
    std::lock_guard<std::mutex> lckhdd(hddInfoMtx);
    return hddInfo->getDeviceActivity();
}

// proc
// lowlevel method
const ProcInfo::LinuxProcinfoMap& InfoManager::getLinuxProcessInfoMap() const
{
    std::lock_guard<std::mutex> lckproc(procInfoMtx);
    return procInfo->getLinuxProcessInfoMap();
}

// highlevel method
const ProcessActivities& InfoManager::getProcessActivities(PID pid)
{
    std::lock_guard<std::mutex> lckproc(procInfoMtx);
    return activitiesMap[pid];
}

// highlevel method
const InfoManager::ActivitiesMap& InfoManager::getActivitiesMap() const
{
    std::lock_guard<std::mutex> lckproc(procInfoMtx);
    return activitiesMap;
}
