#include "cpuwidget.hpp"

CPUWidget::CPUWidget(InfoManager::pointer _infoManager, QWidget *parent)
    : QWidget{parent}, infoManager{_infoManager}
{
    createWidgets();
    createLayout();
    createUpdaterFunctions();
}

void CPUWidget::createWidgets()
{
    totalCPULabel = new QLabel(tr("Total"));
    totalCPUChart = new CPUChart(1);
    cpuLabels.push_back(totalCPULabel);
    cpuCharts.push_back(totalCPUChart);
    cpuCharts.back()->setDrawUnderLine(true);

    int cpuCount = infoManager->cpuCount();

    // 1 because we have already created total chart
    for(int i = 1; i < cpuCount; ++i)
    {
        QString labelText = tr("Core %1").arg(QString::number(i));
        cpuLabels.push_back(new QLabel(labelText));
        cpuCharts.push_back(new CPUChart(1));
        cpuCharts.back()->setDrawUnderLine(true);
    }

    // setting objects names for stylesheeting
    for(int i = 0; i < cpuLabels.size(); ++i)
    {
        cpuLabels[i]->setObjectName(chartDescriptionName);
    }
}

void CPUWidget::createLayout()
{
    QGridLayout* mainLayout = new QGridLayout;
    QVBoxLayout* vbl = new QVBoxLayout;
    vbl->addWidget(cpuLabels[0], 0, Qt::AlignCenter);
    vbl->addWidget(cpuCharts[0]);

    int totalColumns = cpuLabels.size() > 9 ? 4 : 2;
    mainLayout->addLayout(vbl, 0, 0, 1, totalColumns, Qt::AlignCenter);
    // sizes of cpuLabels and cpuCharts should be equal
    for(int i = 1; i < cpuLabels.size(); ++i)
    {
        vbl = new QVBoxLayout;
        vbl->addWidget(cpuLabels[i], 0, Qt::AlignCenter);
        vbl->addWidget(cpuCharts[i]);
        if(cpuLabels.size() <= 2)       // 1 ore less core
        {
            mainLayout->addLayout(vbl, i, 0, 1, 2, Qt::AlignCenter);
        }
        else if (cpuLabels.size() <= 9) // 8 or less cores
        {
            mainLayout->addLayout(vbl, std::ceil((float)i / 2), (i - 1) % 2, 1, 1);
        }
        else                            // more than 8 cores
        {
            mainLayout->addLayout(vbl, std::ceil((float)i / 4), (i - 1) % 4, 1, 1);
        }
    }

    setLayout(mainLayout);
}

/*
    functions for updating CHARTS
*/
void CPUWidget::createUpdaterFunctions()
{
    for(int i = 0; i < cpuCharts.size(); ++i)
    {
        ChartUpdater upd(infoManager, i);
        chartUpdaters.push_back(upd);
        cpuCharts[i]->setUpdaterFunction<ChartUpdater>(upd);
    }
}

// not have data precision
void CPUWidget::setDataPrecision(int){ }

void CPUWidget::setChartGridEnabled(bool on)
{
    for(int i = 0; i < getCpuChartsCount(); ++i)
    {
        getCpuChart(i).setEnableGrid(on);
    }
    update();
}

void CPUWidget::setChartMode(Modes mode)
{
    for(int i = 0; i < getCpuChartsCount(); ++i)
    {
        getCpuChart(i).setMode(mode);
    }
    update();
}

void CPUWidget::stopCharts()
{
    for(int i = 0; i < cpuCharts.size(); ++i)
    {
        cpuCharts[i]->stop();
    }
}

void CPUWidget::restartCharts()
{
    for(int i = 0; i < cpuCharts.size(); ++i)
    {
        cpuCharts[i]->start();
    }
}

