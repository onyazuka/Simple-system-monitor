#pragma once
#include <QtCore>
#include "charts/chartsnamespace.hpp"
#include "applicationnamespace.hpp"

/*
    Application settings
*/
struct Settings
{
    QString cpuStatPath;
    QString memStatPath;
    QString netStatPath;
    int dataPrecision;
    bool gridOn;
    Modes defaultChartsMode;
    Languages language;
};

const Settings DefaultSettings
{
    "/proc/stat",
    "/proc/meminfo",
    "/proc/net/netstat",
    2,
    true,
    Modes::Minute,
    Languages::English
};


