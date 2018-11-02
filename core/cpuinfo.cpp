#include "cpuinfo.hpp"

/*
 * CPU is loaded when it is not idling
*/
float CPUTimes::usage()
{
    uint64_t summ = user + nice + system + idle + iowait + irq + softirq;
    uint64_t idleTime = idle + iowait;
    if(summ == 0)   // division by 0
    {
        return -1;
    }
    return (float)(summ - idleTime) / summ * 100;
}

/*
 * WARNING! Operation is valid only when substraction newer value from older!
*/
CPUTimes CPUTimes::operator-(const CPUTimes& other)
{
    CPUTimes newTimes;
    newTimes.user = user - other.user;
    newTimes.nice = nice - other.nice;
    newTimes.system = system - other.system;
    newTimes.idle = idle - other.idle;
    newTimes.iowait = iowait - other.iowait;
    newTimes.irq = irq - other.irq;
    newTimes.softirq = softirq - other.softirq;
    return newTimes;
}

/*
    Retrieving data from /proc/stat
*/
CPUInfo::CPUInfo(const std::string& statFile)
    : stat{statFile}, _CPUCount{0}
{
    parseStatFile();
}

CPUInfo CPUInfo::fromStatFile(const std::string& statFile)
{
    CPUInfo info(statFile);
    return info;
}

/*
    Throws on error
    Fills current(first measurements)
*/
void CPUInfo::parseStatFile()
{
    std::ifstream ifs{stat};
    if(!ifs)
    {
        throw CPUParseError("CpuInfo::parseStatFile(): can not open file");
    }
    _CPUCount = 0;
    std::string word;
    // reading until 'intr' field
    bool parsed = false;
    while(word != "intr" && ifs)
    {
        ifs >> word;
        if(word.substr(0, 3) == "cpu")
        {
            parsed = true;
            CPUTimes times;
            ifs >> times.user >> times.nice >> times.system >>
                   times.idle >> times.iowait >> times.irq >>
                   times.softirq;
            curMeasurement.push_back(times);
            ++_CPUCount;
            loadPercentages.push_back(times.usage());
        }
    }
    if(!parsed)
    {
        throw CPUParseError("CpuInfo::parseStatFile(): invalid stat file - parsed nothing");
    }
}

/*
    Updates load percentages.
    Takes last(first) measurement, makes current measurement, finds difference between values
        and updates load_percentages.
    throws on error
*/
void CPUInfo::update()
{
    std::ifstream ifs{stat};
    if(!ifs)
    {
        throw CPUParseError("CpuInfo::update(): can not open file");
    }

    lastMeasurement = std::move(curMeasurement);
    curMeasurement.clear(); //just in case

    std::string word;
    // reading until 'intr' field
    bool parsed = false;
    while(word != "intr" && ifs)
    {
        ifs >> word;
        if(word.substr(0, 3) == "cpu")
        {
            parsed = true;
            CPUTimes times;
            ifs >> times.user >> times.nice >> times.system >>
                   times.idle >> times.iowait >> times.irq >>
                   times.softirq;
            curMeasurement.push_back(times);
        }
    }
    if(!parsed)
    {
        throw CPUParseError("CpuInfo::update(): invalid stat file - parsed nothing");
    }

    // updating usages
    for(int i = 0; i < lastMeasurement.size(); ++i)
    {
        float delta = (curMeasurement[i] - lastMeasurement[i]).usage();
        // too small amount of time passed - load is the same
        if(delta == -1)
        {
            continue;
        }
        loadPercentages[i] = delta;
    }
}


