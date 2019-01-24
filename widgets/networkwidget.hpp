#pragma once
#include "emulateablewidget.hpp"
#include "../infomanager.hpp"
#include "../charts/networkchart.hpp"
#include "utils.hpp"
#include "configurablewidget.hpp"
#include "charts/PieChart/labeled_piechart.hpp"

/*-------------------------UPDATERS---------------------------*/

class NetworkIncomeUpdater
{
public:
    NetworkIncomeUpdater() {}
    NetworkIncomeUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager}, lastIncome{_infoManager->getIncome()} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        std::vector<double> res;
        res.push_back(infoManager->getIncome() - lastIncome);
        lastIncome = infoManager->getIncome();
        return res;
    }
private:
    InfoManager::pointer infoManager;
    uint64_t lastIncome;
};

class NetworkOutcomeUpdater
{
public:
    NetworkOutcomeUpdater() {}
    NetworkOutcomeUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager}, lastOutcome{_infoManager->getOutcome()} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        std::vector<double> res;
        res.push_back(infoManager->getOutcome() - lastOutcome);
        lastOutcome = infoManager->getOutcome();
        return res;
    }
private:
    InfoManager::pointer infoManager;
    uint64_t lastOutcome;
};

/*-------------------------/UPDATERS---------------------------*/

/*
    For chart updation we provide updater function.
    *info classes update automatically by InfoManager.
    All others(labels etc) updated by updater() function.
*/
class NetworkWidget : public EmulateableWidget, public ConfigurableWidget
{
    Q_OBJECT
public:
    NetworkWidget(InfoManager::pointer _infoManager, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline NetworkChart& getNetworkIncomeChart() const {return *networkIncomeChart;}
    inline NetworkChart& getNetworkOutcomeChart() const {return *networkOutcomeChart;}

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
    QLabel* incomeInfoLabel;
    QLabel* outcomeInfoLabel;
    NetworkChart* networkIncomeChart;
    NetworkChart* networkOutcomeChart;

// as charts are Emulateable by itselves, and we don't want them to stop when changing tabs,
// we need to provide our own methods for starting/stopping them
public slots:
    void stopCharts();
    void restartCharts();
    void start();

protected slots:
    virtual void updater();
};
