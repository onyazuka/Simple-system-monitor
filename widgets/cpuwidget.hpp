#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "emulateablewidget.hpp"
#include "../core/cpuinfo.hpp"
#include "../charts/percenttimerealtimechart.hpp"
#include "../applicationnamespace.hpp"

/*-------------------------UPDATERS---------------------------*/

class ChartUpdater
{
public:
    ChartUpdater() {}
    ChartUpdater(CPUInfo* info, int ind, bool upd)
    {
        cpuinfo = info;
        index = ind;
        update = upd;
    }
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        if(update) cpuinfo->update();
        std::vector<double> res;
        res.push_back(cpuinfo->getInfo(index));
        return res;
    }
private:
    CPUInfo* cpuinfo;
    int index;
    bool update;
};

/*-------------------------/UPDATERS---------------------------*/

/*
    Contains charts describing CPU activity.
    It is emulateable widget(for convenience) but without manually emulateable elements -
        charts themselves are emulateable(maintain emulation by themselves).
        So we are not using start/stop/updater methods of emulateable widgets.
*/
class CPUWidget : public EmulateableWidget
{
    Q_OBJECT
    typedef PercentTimeRealtimeChart CPUChart;
    typedef QVector<QLabel*> CPULabels;
    typedef QVector<CPUChart*> CPUCharts;
    typedef QVector<ChartUpdater> ChartUpdaters;
public:
    CPUWidget(CPUInfo* _cpuInfo, QWidget* parent=nullptr);
    inline int getCpuChartsCount() const {return cpuCharts.size();}
    inline CPUChart& getCpuChart(int index) const {return *cpuCharts[index];}

private:
    CPUInfo* cpuInfo;
    QLabel* totalCPULabel;
    CPUChart* totalCPUChart;
    CPULabels cpuLabels;
    CPUCharts cpuCharts;
    ChartUpdaters chartUpdaters;

// as charts are Emulateable by itselves, and we don't want them to stop when changing tabs,
// we need to provide our own methods for starting/stopping them
public slots:
    void stopCharts();
    void restartCharts();
    void start() {}
    void stop() {}
};
