#pragma once
#include <stdint.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "parseerror.hpp"

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

    CPUTimes operator-(const CPUTimes& other);
    float usage();
};

/*
    WARNING: cpuInfoCount and size of load vector is always core count + 1
        so 0 record is total cpu information,
        and next is info by cores.
        So if you have 8 cores, cpuInfoCount() will return 9.
    stat is the stat file.
*/
class CPUInfo
{
public:
    CPUInfo(const std::string& statFile = "/proc/stat");
    static CPUInfo fromStatFile(const std::string& statFile = "/proc/stat");
    inline int cpuInfoCount() const {return _CPUCount;}
    inline float getInfo(int index) const {return loadPercentages[index];}
    void update();
    inline const std::string& getCpuStatFileName() const {return stat;}
    inline void setCpuStatFileName(const std::string& _stat) {stat = _stat;}
private:

    inline void addCPU(float perc) {_CPUCount++; loadPercentages.push_back(perc);}
    inline void removeCPU() {_CPUCount--; loadPercentages.pop_back();}
    void parseStatFile();

    std::string stat;
    int _CPUCount;
    std::vector<float> loadPercentages;
    std::vector<CPUTimes> curMeasurement;
    std::vector<CPUTimes> lastMeasurement;
};


