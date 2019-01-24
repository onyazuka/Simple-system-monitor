#pragma once
#include "../infomanager.hpp"
#include "../charts/percenttimerealtimechart.hpp"
#include "configurablewidget.hpp"
#include "charts/PieChart/labeled_piechart.hpp"

/*-------------------------UPDATERS---------------------------*/

class ChartUpdater
{
public:
    ChartUpdater() {}
    ChartUpdater(InfoManager::pointer info, int ind)
        : infoManager{info}, index{ind} {}
    std::vector<double> operator()()
    {
        std::vector<double> res;
        res.push_back(infoManager->getLoad(index));
        return res;
    }
private:
    InfoManager::pointer infoManager;
    int index;
};

/*-------------------------/UPDATERS---------------------------*/

/*
    Contains charts describing CPU activity.
    As long as it stores only charts, it is NOT emulateble.
*/
class CPUWidget : public QWidget, public ConfigurableWidget
{
    Q_OBJECT
    typedef PercentTimeRealtimeChart CPUChart;
    typedef QVector<QLabel*> CPULabels;
    typedef QVector<CPUChart*> CPUCharts;
    typedef QVector<ChartUpdater> ChartUpdaters;
public:
    CPUWidget(InfoManager::pointer _infoManager, QWidget* parent=nullptr);
    inline int getCpuChartsCount() const {return cpuCharts.size();}
    inline CPUChart& getCpuChart(int index) const {return *cpuCharts[index];}

private:
    void createWidgets();
    void createLayout();
    void createUpdaterFunctions();

    // settings setters
    void setDataPrecision(int prec);
    void setChartGridEnabled(bool on);
    void setChartMode(Modes mode);

    InfoManager::pointer infoManager;
    QLabel* totalCPULabel;
    CPUChart* totalCPUChart;
    CPULabels cpuLabels;
    CPUCharts cpuCharts;
    ChartUpdaters chartUpdaters;

public slots:
    void stopCharts();
    void restartCharts();

};
