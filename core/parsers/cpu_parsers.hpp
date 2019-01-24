#pragma once
#include "parser.hpp"
#include <vector>

/*
    Has values of CPU state in this moment(times for different activities).
*/
struct CPUTimes
{
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;

    uint64_t summ() const
    {
        return user + nice + system + idle + iowait + irq + softirq;
    }
    CPUTimes operator-(const CPUTimes& other);
    float usage();
};


class CPUParser : public Parser
{
public:
    typedef std::unique_ptr<CPUParser> pointer;
    virtual void parse() = 0;
    virtual void update() = 0;
    virtual float getLoad(size_t core) const = 0;
    virtual size_t getCpuCount() const = 0;
    virtual uint64_t getTotalWorkTime() const = 0;
};

class StatFileCPUParser : public CPUParser
{
public:
    typedef std::unique_ptr<StatFileCPUParser> pointer;
    typedef std::vector<CPUTimes> Measurements;
    StatFileCPUParser(const std::string& fname = "/proc/stat");
    void parse() final;
    void update() final;
    float getLoad(size_t core) const final;
    inline virtual size_t getCpuCount() const { return _CPUCount; }
    uint64_t getTotalWorkTime() const;
    inline const std::string& getStatFileName() const { return statFile; }
private:
    inline void addCPU(float perc) {_CPUCount++; loadPercentages.push_back(perc);}
    inline void removeCPU() {_CPUCount--; loadPercentages.pop_back();}
    int _CPUCount;
    std::string statFile;
    std::vector<float> loadPercentages;
    Measurements curMeasurement;
    Measurements lastMeasurement;
    CPUTimes lastCPUTimes;
};
