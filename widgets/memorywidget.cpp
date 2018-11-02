#include "memorywidget.hpp"

MemoryWidget::MemoryWidget(MemInfo* _meminfo, QWidget* parent)
    : EmulateableWidget{parent}, dataPrecision{2}, meminfo{_meminfo}
{
    // creating widgets
    setUpdateInterval(500);
    freeMemoryInfoLabel = new QLabel;
    freeSwapInfoLabel = new QLabel;
    memoryUsageChart = new MemoryChart(1);
    swapUsageChart = new MemoryChart(1);

    // setting object names for stylesheeting
    freeMemoryInfoLabel->setObjectName(chartDescriptionName);
    freeSwapInfoLabel->setObjectName(chartDescriptionName);

    // layouting
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

    // providing updaters
    MemoryUsageUpdater muu(meminfo, true);
    SwapUsageUpdater   suu(meminfo, false);
    memoryUsageChart->setUpdaterFunction(muu);
    swapUsageChart->setUpdaterFunction(suu);
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
    if(!meminfo) return;
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...) {}
}

void MemoryWidget::updater()
{
    // probably it is bad that we updating this both from updater function and updater method of class
    meminfo->update();
    // multiplying by 1024 because /proc/memstat provides data in kilobytes
    QString label = tr("Memory free: ") + Translator::fitBytes(meminfo->memAvailable * 1024, dataPrecision);
    freeMemoryInfoLabel->setText(label);
    label = tr("Swap free: ") + Translator::fitBytes(meminfo->swapFree * 1024, dataPrecision);
    freeSwapInfoLabel->setText(label);
}
