#pragma once
#include "applicationnamespace.hpp"
#include "charts/chartsnamespace.hpp"
#include "settings.hpp"

/*
    If some widget is not planning to implement some option, simply leave the definition empty.
*/
class ConfigurableWidget
{
public:
    void setSettings(const Settings& _settings);
protected:
    // settings setters
    virtual void setDataPrecision(int prec) = 0;
    virtual void setChartGridEnabled(bool on) = 0;
    virtual void setChartMode(Modes mode) = 0;
};
