#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "emulateablewidget.hpp"
#include "../core/meminfo.hpp"
#include "../charts/percenttimerealtimechart.hpp"
#include "utils.hpp"
#include "../applicationnamespace.hpp"

/*-------------------------UPDATERS---------------------------*/

class MemoryUsageUpdater
{
public:
    MemoryUsageUpdater() {}
    MemoryUsageUpdater(MemInfo* info, bool upd)
        : meminfo{info}, update{upd} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        // probably it is bad that we updating this both from updater function and updater method of class
        if(update) meminfo->update();
        std::vector<double> res;
        res.push_back(meminfo->memoryUsage());
        return res;
    }
private:
    MemInfo* meminfo;
    bool update;
};

class SwapUsageUpdater
{
public:
    SwapUsageUpdater() {}
    SwapUsageUpdater(MemInfo* info, bool upd)
        : meminfo{info}, update{upd} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        if(update) *meminfo = MemInfo::fromMemInfoFile();
        std::vector<double> res;
        res.push_back(meminfo->swapUsage());
        return res;
    }
private:
    MemInfo* meminfo;
    bool update;
};

/*-------------------------/UPDATERS---------------------------*/

class MemoryWidget : public EmulateableWidget
{
    Q_OBJECT
    typedef PercentTimeRealtimeChart MemoryChart;
public:
    MemoryWidget(MemInfo* _meminfo, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline void setDataPrecision(int precision) {dataPrecision = precision;}
    inline MemoryChart& getMemoryUsageChart() const { return *memoryUsageChart; }
    inline MemoryChart& getSwapUsageChart() const { return *swapUsageChart; }

private:
    //settings
    int dataPrecision;

    //widgets
    MemInfo* meminfo;
    QLabel* freeMemoryInfoLabel;
    QLabel* freeSwapInfoLabel;
    MemoryChart* memoryUsageChart;
    MemoryChart* swapUsageChart;

// as charts are Emulateable by itselves, and we don't want them to stop when changing tabs,
// we need to provide our own methods for starting/stopping them
public slots:
    void stopCharts();
    void restartCharts();
    void start();

protected slots:
    virtual void updater();
};
