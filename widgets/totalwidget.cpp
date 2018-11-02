#include "systemmonitor.hpp"

TotalWidget::TotalWidget(CPUInfo* _cpuInfo, MemInfo* _meminfo, NetInfo* _netInfo, QWidget *parent)
    :  EmulateableWidget{parent}, cpuinfo{_cpuInfo}, meminfo{_meminfo},
      netInfo{_netInfo},dataPrecision{2}
{

    setUpdateInterval(500);
    coreCount = (cpuinfo == nullptr) ? 1 : cpuinfo->cpuInfoCount() - 1;

    memoryLabel =           new QLabel(tr("Memory"));
    memoryTotalLabel =      new QLabel(tr("Total: "));
    memoryTotalInfoLabel =  new QLabel;
    memoryAvailLabel =      new QLabel(tr("Available: "));
    memoryAvailInfoLabel =  new QLabel;
    memoryAvailPB =         new QProgressBar;
    memoryUsageLabel =      new QLabel(tr("Usage: "));
    memoryUsageInfoLabel =  new QLabel;
    memoryUsagePB =         new QProgressBar;
    memoryUsagePB->setTextVisible(false);

    swapLabel =             new QLabel(tr("Swap"));
    swapTotalLabel =        new QLabel(tr("Total: "));
    swapTotalInfoLabel =    new QLabel;
    swapAvailLabel =        new QLabel(tr("Available: "));
    swapAvailInfoLabel =    new QLabel;
    swapAvailPB =           new QProgressBar;

    cpuLabel =              new QLabel(tr("CPU"));
    cpuTotalLabel =         new QLabel(tr("Total: "));
    cpuTotalInfoLabel =     new QLabel;

    networkLabel =          new QLabel(tr("Network"));
    networkIncomeLabel =    new QLabel(tr("Incoming: "));
    networkIncomeInfoLabel =new QLabel;
    networkOutcomeLabel =   new QLabel(tr("Outgoing: "));
    networkOutcomeInfoLabel=new QLabel;

    // object names used in css
    memoryLabel->setObjectName(titleLabelsName);
    swapLabel->setObjectName(titleLabelsName);
    cpuLabel->setObjectName(titleLabelsName);
    networkLabel->setObjectName(titleLabelsName);
    memoryTotalLabel->setObjectName(propertyLabelsName);
    memoryAvailLabel->setObjectName(propertyLabelsName);
    memoryUsageLabel->setObjectName(propertyLabelsName);
    swapTotalLabel->setObjectName(propertyLabelsName);
    swapAvailLabel->setObjectName(propertyLabelsName);
    cpuTotalLabel->setObjectName(propertyLabelsName);
    networkIncomeLabel->setObjectName(propertyLabelsName);
    networkOutcomeLabel->setObjectName(propertyLabelsName);
    memoryTotalInfoLabel->setObjectName(infoLabelsName);
    memoryAvailInfoLabel->setObjectName(infoLabelsName);
    memoryUsageInfoLabel->setObjectName(infoLabelsName);
    swapTotalInfoLabel->setObjectName(infoLabelsName);
    swapAvailInfoLabel->setObjectName(infoLabelsName);
    cpuTotalInfoLabel->setObjectName(infoLabelsName);
    networkIncomeInfoLabel->setObjectName(infoLabelsName);
    networkOutcomeInfoLabel->setObjectName(infoLabelsName);

    QFrame* memorySplitter = new QFrame;
    memorySplitter->setFrameShape(QFrame::HLine);
    memorySplitter->setFrameShadow(QFrame::Sunken);
    QFrame* swapSplitter = new QFrame;
    swapSplitter->setFrameShape(QFrame::HLine);
    swapSplitter->setFrameShadow(QFrame::Sunken);
    QFrame* cpuSplitter = new QFrame;
    cpuSplitter->setFrameShape(QFrame::HLine);
    cpuSplitter->setFrameShadow(QFrame::Sunken);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(memoryLabel, 0, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addWidget(memoryTotalLabel, 1, 0, 1, 1);
    mainLayout->addWidget(memoryTotalInfoLabel, 1, 1, 1, 1);
    mainLayout->addWidget(memoryAvailLabel, 2, 0, 1, 1);
    mainLayout->addWidget(memoryAvailInfoLabel, 2, 1, 1, 1);
    mainLayout->addWidget(memoryAvailPB, 2, 2, 1, 2);
    mainLayout->addWidget(memoryUsageLabel, 3, 0, 1, 1);
    mainLayout->addWidget(memoryUsageInfoLabel, 3, 1, 1, 1);
    mainLayout->addWidget(memoryUsagePB, 3, 2, 1, 2);
    mainLayout->addWidget(memorySplitter, 4, 0, 1, 4);
    mainLayout->addWidget(swapLabel, 5, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addWidget(swapTotalLabel, 6, 0, 1, 1);
    mainLayout->addWidget(swapTotalInfoLabel, 6, 1, 1, 1);
    mainLayout->addWidget(swapAvailLabel, 7, 0, 1, 1);
    mainLayout->addWidget(swapAvailInfoLabel, 7, 1, 1, 1);
    mainLayout->addWidget(swapAvailPB, 7, 2, 1, 2);
    mainLayout->addWidget(swapSplitter, 8, 0, 1, 4);
    mainLayout->addWidget(cpuLabel, 9, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addWidget(cpuTotalLabel, 10, 0, 1, 1);
    mainLayout->addWidget(cpuTotalInfoLabel, 10, 1, 1, 1);

    int curRow = 11;
    // adding core labels
    for(int i = 0; i < coreCount; ++i)
    {
        QString description = QString(tr("Core ")) + QString::number(i + 1) + QString(": ");
        coreLabels.push_back(CoreLabelsPair(new QLabel(description), new QLabel));
        corePBs.push_back(new QProgressBar);
        corePBs[corePBs.size() - 1]->setTextVisible(false);
        coreLabels.back().first->setObjectName(propertyLabelsName);
        coreLabels.back().second->setObjectName(infoLabelsName);
        mainLayout->addWidget(coreLabels.back().first, curRow, 0, 1, 1);
        mainLayout->addWidget(coreLabels.back().second, curRow, 1, 1, 1);
        mainLayout->addWidget(corePBs.back(), curRow, 2, 1, 2);
        ++curRow;
    }

    mainLayout->addWidget(cpuSplitter, curRow++, 0, 1, 4);
    mainLayout->addWidget(networkLabel, curRow++, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addWidget(networkIncomeLabel, curRow, 0, 1, 1);
    mainLayout->addWidget(networkIncomeInfoLabel, curRow++, 1, 1, 1);
    mainLayout->addWidget(networkOutcomeLabel, curRow, 0, 1, 1);
    mainLayout->addWidget(networkOutcomeInfoLabel, curRow++, 1, 1, 1);

    setLayout(mainLayout);

    // configuring progress bars
    for(auto child : children())
    {
        QProgressBar* pb = qobject_cast<QProgressBar*>(child);
        if(pb == 0)
        {
            continue;
        }
        pb->setRange(0, 100);
    }
}

/*
    Tries to continue or start work.
    If it can not, does nothing.
*/
void TotalWidget::start()
{
    if(!cpuinfo || !meminfo || !netInfo)
    {
        return;
    }
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...) {}
}

/*
    Private method that updates labels.
*/
void TotalWidget::updater()
{
    // updating values
    meminfo->update();
    cpuinfo->update();
    netInfo->update();

    // updating labels
    // multiplying by 1024 because /proc/memstat provides data in kilobytes
    memoryTotalInfoLabel->setText(Translator::fitBytes(meminfo->memTotal * 1024, dataPrecision));
    memoryAvailInfoLabel->setText(Translator::fitBytes(meminfo->memAvailable * 1024, dataPrecision));
    memoryAvailPB->setValue((float)meminfo->memAvailable / meminfo->memTotal * 100);
    memoryUsageInfoLabel->setText(QString::number(meminfo->memoryUsage(), 'd', dataPrecision) + "%");
    memoryUsagePB->setValue(std::round(meminfo->memoryUsage()));
    swapTotalInfoLabel->setText(Translator::fitBytes(meminfo->swapTotal * 1024, dataPrecision));
    swapAvailInfoLabel->setText(Translator::fitBytes(meminfo->swapFree * 1024, dataPrecision));
    swapAvailPB->setValue((float)meminfo->swapFree / meminfo->swapTotal * 100);
    cpuTotalInfoLabel->setText(QString::number(cpuinfo->getInfo(0), 'd', dataPrecision) + "%");
    for(int i = 0; i < coreCount; ++i)
    {
        float usage = cpuinfo->getInfo(i + 1);
        coreLabels[i].second->setText(QString::number(usage, 'd', dataPrecision) + "%");
        corePBs[i]->setValue(std::round(usage));
    }
    networkIncomeInfoLabel->setText(Translator::fitBytes(netInfo->getIncome(), dataPrecision));
    networkOutcomeInfoLabel->setText(Translator::fitBytes(netInfo->getOutcome(), dataPrecision));
}
