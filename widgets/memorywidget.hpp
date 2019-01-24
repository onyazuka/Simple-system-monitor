#pragma once
#include "emulateablewidget.hpp"
#include "../infomanager.hpp"
#include "../charts/percenttimerealtimechart.hpp"
#include "utils.hpp"
#include "configurablewidget.hpp"
#include "charts/PieChart/labeled_piechart.hpp"

/*-------------------------UPDATERS---------------------------*/

class MemoryUsageUpdater
{
public:
    MemoryUsageUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager} {}
    std::vector<double> operator()()
    {
        std::vector<double> res;
        res.push_back(infoManager->getMemoryUsage());
        return res;
    }
private:
    InfoManager::pointer infoManager;
};

class SwapUsageUpdater
{
public:
    SwapUsageUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager} {}
    std::vector<double> operator()()
    {
        std::vector<double> res;
        res.push_back(infoManager->getSwapUsage());
        return res;
    }
private:
    InfoManager::pointer infoManager;
};

/*-------------------------/UPDATERS---------------------------*/

/*
    For chart updation we provide updater function.
    *info classes update automatically by InfoManager.
    All others(labels etc) updated by updater() function.
*/
class MemoryWidget : public EmulateableWidget, public ConfigurableWidget
{
    Q_OBJECT
    typedef PercentTimeRealtimeChart MemoryChart;
public:
    MemoryWidget(InfoManager::pointer _infoManager, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline MemoryChart& getMemoryUsageChart() const { return *memoryUsageChart; }
    inline MemoryChart& getSwapUsageChart() const { return *swapUsageChart; }

private:
    void createWidgets();
    void createLayout();
    void createUpdaterFunctions();

    // settings setters
    void setDataPrecision(int prec);
    void setChartGridEnabled(bool on);
    void setChartMode(Modes mode);

    //settings
    int dataPrecision;

    //widgets
    InfoManager::pointer infoManager;
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
