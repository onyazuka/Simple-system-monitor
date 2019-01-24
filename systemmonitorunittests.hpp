#pragma once
#include "systemmonitor.hpp"

class SystemMonitorTest : public QObject
{
    Q_OBJECT
private slots:
    void Settings_Test();
    void WithoutSomeWidgets_Test();
};
