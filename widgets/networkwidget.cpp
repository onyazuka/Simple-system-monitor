#include "networkwidget.hpp"

NetworkWidget::NetworkWidget(InfoManager::pointer _infoManager, QWidget* parent)
    : EmulateableWidget{parent}, dataPrecision{2}, infoManager{_infoManager}
{
    setUpdateInterval(500);
    createWidgets();
    createLayout();
    createUpdaterFunctions();
}

void NetworkWidget::createWidgets()
{
    incomeInfoLabel = new QLabel;
    outcomeInfoLabel = new QLabel;
    networkIncomeChart = new NetworkChart(1);
    networkOutcomeChart = new NetworkChart(1);

    // setting object names for stylesheeting
    incomeInfoLabel->setObjectName(chartDescriptionName);
    outcomeInfoLabel->setObjectName(chartDescriptionName);
}

void NetworkWidget::createLayout()
{
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
}

void NetworkWidget::createUpdaterFunctions()
{
    NetworkIncomeUpdater niu(infoManager);
    NetworkOutcomeUpdater nou(infoManager);
    networkIncomeChart->setUpdaterFunction(niu);
    networkOutcomeChart->setUpdaterFunction(nou);
}

// settings setters
void NetworkWidget::setDataPrecision(int prec)
{
    dataPrecision = prec;
    getNetworkIncomeChart().setDataPrecision(prec);
    getNetworkOutcomeChart().setDataPrecision(prec);
    update();
}

void NetworkWidget::setChartGridEnabled(bool on)
{
    getNetworkIncomeChart().setEnableGrid(on);
    getNetworkOutcomeChart().setEnableGrid(on);
    update();
}

void NetworkWidget::setChartMode(Modes mode)
{
    getNetworkIncomeChart().setMode(mode);
    getNetworkOutcomeChart().setMode(mode);
    update();
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
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...)
    {
        qWarning() << "Network widget is not working";
    }
}

void NetworkWidget::updater()
{
    QString label = tr("Incoming: ") + Translator::fitBytes(infoManager->getIncome(), dataPrecision);
    incomeInfoLabel->setText(label);
    label = tr("Outgoing: ") + Translator::fitBytes(infoManager->getOutcome(), dataPrecision);
    outcomeInfoLabel->setText(label);
}
