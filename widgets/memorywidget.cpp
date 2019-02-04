#include "memorywidget.hpp"

MemoryWidget::MemoryWidget(InfoManager::pointer _infoManager, QWidget* parent)
    : EmulateableWidget{parent}, dataPrecision{2}, infoManager{_infoManager}
{
    setUpdateInterval(500);
    createWidgets();
    createLayout();
    createUpdaterFunctions();
}

void MemoryWidget::createWidgets()
{
    freeMemoryInfoLabel = new QLabel;
    freeSwapInfoLabel = new QLabel;
    memoryUsageChart = new MemoryChart(1);
    memoryUsageChart->setDrawUnderLine(true);
    swapUsageChart = new MemoryChart(1);
    swapUsageChart->setDrawUnderLine(true);

    // setting object names for stylesheeting
    freeMemoryInfoLabel->setObjectName(chartDescriptionName);
    freeSwapInfoLabel->setObjectName(chartDescriptionName);
}

void MemoryWidget::createLayout()
{
    QVBoxLayout* vbl = new QVBoxLayout;
    QHBoxLayout* memhbl = new QHBoxLayout;
    memhbl->addWidget(freeMemoryInfoLabel);
    vbl->addLayout(memhbl);
    vbl->setAlignment(memhbl, Qt::AlignCenter);
    vbl->addWidget(memoryUsageChart, 2);
    QHBoxLayout* swaphbl = new QHBoxLayout;
    swaphbl->addWidget(freeSwapInfoLabel);
    vbl->addLayout(swaphbl);
    vbl->setAlignment(swaphbl, Qt::AlignCenter);
    vbl->addWidget(swapUsageChart, 2);
    setLayout(vbl);
}

void MemoryWidget::createUpdaterFunctions()
{
    MemoryUsageUpdater muu(infoManager);
    SwapUsageUpdater   suu(infoManager);
    memoryUsageChart->setUpdaterFunction(muu);
    swapUsageChart->setUpdaterFunction(suu);
}

// settings setters
void MemoryWidget::setDataPrecision(int prec)
{
    dataPrecision = prec;
    update();
}

void MemoryWidget::setChartGridEnabled(bool on)
{
    getMemoryUsageChart().setEnableGrid(on);
    getSwapUsageChart().setEnableGrid(on);
    update();
}

void MemoryWidget::setChartMode(Modes mode)
{
    getMemoryUsageChart().setMode(mode);
    getSwapUsageChart().setMode(mode);
    update();
}

void MemoryWidget::stopCharts()
{
    memoryUsageChart->stop();
    swapUsageChart->stop();
}

void MemoryWidget::restartCharts()
{
    memoryUsageChart->start();
    swapUsageChart->start();
}

/*
    Tries to continue or start work.
    If it can not, does nothing.
*/
void MemoryWidget::start()
{
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...)
    {
        qWarning() << "Memory widget is not working";
    }
}

void MemoryWidget::updater()
{
    // multiplying by 1024 because /proc/memstat provides data in kilobytes
    QString label = tr("Memory free: ") + Translator::fitBytes(infoManager->getMemoryAvailable() * 1024, dataPrecision);
    freeMemoryInfoLabel->setText(label);
    label = tr("Swap free: ") + Translator::fitBytes(infoManager->getSwapAvailable() * 1024, dataPrecision);
    freeSwapInfoLabel->setText(label);
}
