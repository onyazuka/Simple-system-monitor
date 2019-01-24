#pragma once
#include <QtCore>
#include "charts/chartsnamespace.hpp"
#include "applicationnamespace.hpp"

/*
    Application settings
*/
struct Settings
{
    int dataPrecision;
    bool gridOn;
    Modes defaultChartsMode;
    Languages language;
    bool enableCpu;
    bool enableMem;
    bool enableNet;
    bool enableHdd;
    bool enableProc;
};

const Settings DefaultSettings
{
    2,
    true,
    Modes::Minute,
    Languages::English,
    true,
    true,
    true,
    true,
    true
};


