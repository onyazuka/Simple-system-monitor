#include "networkwidget.hpp"

NetworkWidget::NetworkWidget(NetInfo* _netinfo, QWidget* parent)
    : EmulateableWidget{parent}, dataPrecision{2}, netinfo{_netinfo}
{
    // creating widgets
    setUpdateInterval(500);
    incomeInfoLabel = new QLabel;
    outcomeInfoLabel = new QLabel;
    networkIncomeChart = new NetworkChart(1);
    networkOutcomeChart = new NetworkChart(1);

    // setting object names for stylesheeting
    incomeInfoLabel->setObjectName(chartDescriptionName);
    outcomeInfoLabel->setObjectName(chartDescriptionName);

    // layouting
    QVBoxLayout* vbl = new QVBoxLayout;
    QHBoxLayout* memhbl = new QHBoxLayout;
    memhbl->addWidget(incomeInfoLabel);
    vbl->addLayout(memhbl);
    vbl->setAlignment(memhbl, Qt::AlignCenter);
    vbl->addWidget(networkIncomeChart, 2);
    QHBoxLayout* swaphbl = new QHBoxLayout;
    swaphbl->addWidget(outcomeInfoLabel);
    vbl->addLayout(swaphbl);
    vbl->setAlignment(swaphbl, Qt::AlignCenter);
    vbl->addWidget(networkOutcomeChart, 2);
    setLayout(vbl);

    // providing updaters
    NetworkIncomeUpdater niu(netinfo, true);
    NetworkOutcomeUpdater nou(netinfo, false);
    networkIncomeChart->setUpdaterFunction(niu);
    networkOutcomeChart->setUpdaterFunction(nou);
}

void NetworkWidget::stopCharts()
{
    networkIncomeChart->stop();
    networkOutcomeChart->stop();
}

void NetworkWidget::restartCharts()
{
    networkIncomeChart->start();
    networkOutcomeChart->start();
}

/*
    Tries to continue or start work.
    If it can not, does nothing.
*/
void NetworkWidget::start()
{
    if(!netinfo) return;
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...) {}
}

void NetworkWidget::updater()
{
    // probably it is bad that we updating this both from updater function and updater method of class
    netinfo->update();
    QString label = tr("Incoming: ") + Translator::fitBytes(netinfo->getIncome(), dataPrecision);
    incomeInfoLabel->setText(label);
    label = tr("Outgoing: ") + Translator::fitBytes(netinfo->getOutcome(), dataPrecision);
    outcomeInfoLabel->setText(label);
}
