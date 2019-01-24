#include "cpu_parsers.hpp"

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

StatFileCPUParser::StatFileCPUParser(const std::string &fname)
    : statFile{fname} {}

/*
    Throws on error
    Fills current(first measurements)
*/
void StatFileCPUParser::parse()
{
    std::ifstream ifs{statFile};
    if(!ifs)
    {
        throw CPUParseError("StatFileCPUParser::parse(): can not open file");
    }
    curMeasurement.clear();
    loadPercentages.clear();
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
            lastCPUTimes = std::move(times);
        }
    }
    if(!parsed)
    {
        throw CPUParseError("StatFileCPUParser::parse(): invalid stat file - parsed nothing");
    }
}


/*
    Updates load percentages.
    Takes last(first) measurement, makes current measurement, finds difference between values
        and updates load_percentages.
    throws on error
*/
void StatFileCPUParser::update()
{
    lastMeasurement = std::move(curMeasurement);
    parse();
    // updating usages
    for(size_t i = 0; i < lastMeasurement.size(); ++i)
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

float StatFileCPUParser::getLoad(size_t core) const
{
    return loadPercentages[core];
}

uint64_t StatFileCPUParser::getTotalWorkTime() const
{
    return lastCPUTimes.idle +
            lastCPUTimes.iowait +
            lastCPUTimes.irq +
            lastCPUTimes.nice +
            lastCPUTimes.softirq +
            lastCPUTimes.system +
            lastCPUTimes.user;
}
