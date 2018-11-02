#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "emulateablewidget.hpp"
#include "../core/netinfo.hpp"
#include "../charts/networkchart.hpp"
#include "utils.hpp"
#include "../applicationnamespace.hpp"

/*-------------------------UPDATERS---------------------------*/

class NetworkIncomeUpdater
{
public:
    NetworkIncomeUpdater() {}
    NetworkIncomeUpdater(NetInfo* info, bool upd)
        : netinfo{info}, update{upd}, lastIncome{info->getIncome()} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        // probably it is bad that we updating this both from updater function and updater method of class
        if(update) netinfo->update();
        std::vector<double> res;
        res.push_back(netinfo->getIncome() - lastIncome);
        lastIncome = netinfo->getIncome();
        return res;
    }
private:
    NetInfo* netinfo;
    bool update;
    uint64_t lastIncome;
};

class NetworkOutcomeUpdater
{
public:
    NetworkOutcomeUpdater() {}
    NetworkOutcomeUpdater(NetInfo* info, bool upd)
        : netinfo{info}, update{upd}, lastOutcome{info->getOutcome()} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        // probably it is bad that we updating this both from updater function and updater method of class
        if(update) netinfo->update();
        std::vector<double> res;
        res.push_back(netinfo->getOutcome() - lastOutcome);
        lastOutcome = netinfo->getOutcome();
        return res;
    }
private:
    NetInfo* netinfo;
    bool update;
    uint64_t lastOutcome;
};

/*-------------------------/UPDATERS---------------------------*/

class NetworkWidget : public EmulateableWidget
{
    Q_OBJECT
public:
    NetworkWidget(NetInfo* _netinfo, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline void setDataPrecision(int precision) {dataPrecision = precision;}
    inline NetworkChart& getNetworkIncomeChart() const {return *networkIncomeChart;}
    inline NetworkChart& getNetworkOutcomeChart() const {return *networkOutcomeChart;}

private:
    //settings
    int dataPrecision;

    //widgets
    NetInfo* netinfo;
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
