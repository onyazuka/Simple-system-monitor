#include "systemmonitor.hpp"

TotalWidget::TotalWidget(InfoManager::pointer _infoManager, QWidget *parent)
    :  EmulateableWidget{parent}, infoManager{_infoManager},dataPrecision{2}
{
    setUpdateInterval(500);
    createWidgets();
    createLayout();
}

void TotalWidget::createWidgets()
{
    coreCount = (!infoManager->isCpuInfoInitialized()) ? 0 : infoManager->cpuCount() - 1;

    memoryLabel =           new QLabel(tr("Memory"));
    memoryTotalLabel =      new QLabel(tr("Total: "));
    memoryTotalInfoLabel =  new QLabel;
    memoryAvailPieChart =   new LabeledPiechart(LabeledPiechart::Direction::Right);
    memoryAvailPieChart->getPieChart()->setFixedSize(100,100);
    memoryAvailPieChart->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    memoryAvailPieChart->getPieChart()->setPen(QPen(QColor(0,0,0,128)));

    swapLabel =             new QLabel(tr("Swap"));
    swapTotalLabel =        new QLabel(tr("Total: "));
    swapTotalInfoLabel =    new QLabel;
    swapAvailPieChart =     new LabeledPiechart(LabeledPiechart::Direction::Right);
    swapAvailPieChart->getPieChart()->setFixedSize(100,100);
    swapAvailPieChart->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    swapAvailPieChart->getPieChart()->setPen(QPen(QColor(0,0,0,128)));

    cpuLabel =              new QLabel(tr("CPU"));
    cpuTotalLabel =         new QLabel(tr("Total: "));
    cpuTotalInfoLabel =     new QLabel;

    networkLabel =          new QLabel(tr("Network"));
    networkIncomeLabel =    new QLabel(tr("Incoming: "));
    networkIncomeInfoLabel =new QLabel;
    networkOutcomeLabel =   new QLabel(tr("Outgoing: "));
    networkOutcomeInfoLabel=new QLabel;

    hddLabel =              new QLabel(tr("HDD"));
    hddReadLabel =          new QLabel(tr("Read: "));
    hddReadInfoLabel =      new QLabel;
    hddWriteLabel =         new QLabel(tr("Write: "));
    hddWriteInfoLabel =     new QLabel;

    // object names used in css
    memoryLabel->setObjectName(titleLabelsName);
    swapLabel->setObjectName(titleLabelsName);
    cpuLabel->setObjectName(titleLabelsName);
    networkLabel->setObjectName(titleLabelsName);
    memoryTotalLabel->setObjectName(propertyLabelsName);
    swapTotalLabel->setObjectName(propertyLabelsName);
    cpuTotalLabel->setObjectName(propertyLabelsName);
    networkIncomeLabel->setObjectName(propertyLabelsName);
    networkOutcomeLabel->setObjectName(propertyLabelsName);
    memoryTotalInfoLabel->setObjectName(infoLabelsName);
    swapTotalInfoLabel->setObjectName(infoLabelsName);
    cpuTotalInfoLabel->setObjectName(infoLabelsName);
    networkIncomeInfoLabel->setObjectName(infoLabelsName);
    networkOutcomeInfoLabel->setObjectName(infoLabelsName);
    hddLabel->setObjectName(titleLabelsName);
    hddReadLabel->setObjectName(propertyLabelsName);
    hddWriteLabel->setObjectName(propertyLabelsName);
    hddReadInfoLabel->setObjectName(infoLabelsName);
    hddWriteInfoLabel->setObjectName(infoLabelsName);

    memorySplitter = new QFrame;
    memorySplitter->setFrameShape(QFrame::HLine);
    memorySplitter->setFrameShadow(QFrame::Sunken);
    swapSplitter = new QFrame;
    swapSplitter->setFrameShape(QFrame::HLine);
    swapSplitter->setFrameShadow(QFrame::Sunken);
    cpuSplitter = new QFrame;
    cpuSplitter->setFrameShape(QFrame::HLine);
    cpuSplitter->setFrameShadow(QFrame::Sunken);
    netSplitter = new QFrame;
    netSplitter->setFrameShape(QFrame::HLine);
    netSplitter->setFrameShadow(QFrame::Sunken);

    for(int i = 0; i < coreCount; ++i)
    {
        QString description = QString(tr("Core ")) + QString::number(i + 1) + QString(": ");
        coreLabels.push_back(CoreLabelsPair(new QLabel(description), new QLabel));
        QProgressBar* newPB = new QProgressBar;
        newPB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        newPB->setOrientation(Qt::Vertical);
        corePBs.push_back(newPB);
        corePBs[corePBs.size() - 1]->setTextVisible(false);
        coreLabels.back().first->setObjectName(propertyLabelsName);
        coreLabels.back().second->setObjectName(infoLabelsName);
    }
}

void TotalWidget::createLayout()
{
    QGridLayout* mainLayout = new QGridLayout;
    int row = 0;
    QVBoxLayout* memoryTotalSublayout = new QVBoxLayout;
    QVBoxLayout* swapTotalSublayout = new QVBoxLayout;

    memoryTotalSublayout->addWidget(memoryTotalLabel);
    memoryTotalSublayout->addWidget(memoryTotalInfoLabel);
    swapTotalSublayout->addWidget(swapTotalLabel);
    swapTotalSublayout->addWidget(swapTotalInfoLabel);

    mainLayout->addWidget(memoryLabel, row++, 0, 1, 1);
    mainLayout->addLayout(memoryTotalSublayout, row, 0, 1, 1, Qt::AlignTop);
    mainLayout->addWidget(memoryAvailPieChart, row++, 1, 1, 1);
    mainLayout->addWidget(memorySplitter, row++, 0, 1, 4);
    mainLayout->addWidget(swapLabel, row++, 0, 1, 1);
    mainLayout->addLayout(swapTotalSublayout, row, 0, 1, 1, Qt::AlignTop);
    mainLayout->addWidget(swapAvailPieChart, row++, 1, 1, 1);
    mainLayout->addWidget(swapSplitter, row++, 0, 1, 4);

    mainLayout->addWidget(cpuLabel, row++, 0, 1, 1);

    QHBoxLayout* coresSublayout = new QHBoxLayout;
    // adding core labels
    for(int i = 0; i < coreCount; ++i)
    {
        QVBoxLayout* coresSubSublayout = new QVBoxLayout;
        coresSubSublayout->addWidget(coreLabels[i].first, 1, Qt::AlignCenter);
        coresSubSublayout->addWidget(corePBs[i], 5);
        coresSubSublayout->addWidget(coreLabels[i].second, 1, Qt::AlignCenter);
        coresSublayout->addLayout(coresSubSublayout, Qt::AlignCenter);
        if((i % 8 == 7) && (i != coreCount - 1))
        {
            mainLayout->addLayout(coresSublayout, row++, 0, 1, 4);
            QFrame* tempSplitter = new QFrame();
            tempSplitter->setFrameShape(QFrame::HLine);
            tempSplitter->setFrameShadow(QFrame::Sunken);
            mainLayout->addWidget(tempSplitter, row++, 1, 1, 2);
            coresSublayout = new QHBoxLayout();
        }
        else if(i == (coreCount - 1))
        {
            mainLayout->addLayout(coresSublayout, row, 0, 1, 4);
            ++row;
        }
    }
    mainLayout->addWidget(cpuSplitter, row++, 0, 1, 4);
    mainLayout->addWidget(networkLabel, row++, 0, 1, 4);
    mainLayout->addWidget(networkIncomeLabel, row, 0, 1, 1);
    mainLayout->addWidget(networkIncomeInfoLabel, row++, 1, 1, 1);
    mainLayout->addWidget(networkOutcomeLabel, row, 0, 1, 1);
    mainLayout->addWidget(networkOutcomeInfoLabel, row++, 1, 1, 1);
    mainLayout->addWidget(netSplitter, row++, 0, 1, 4);
    mainLayout->addWidget(hddLabel, row++, 0, 1, 1);
    mainLayout->addWidget(hddReadLabel, row, 0, 1, 1);
    mainLayout->addWidget(hddReadInfoLabel, row++, 1, 1, 1);
    mainLayout->addWidget(hddWriteLabel, row, 0, 1, 1);
    mainLayout->addWidget(hddWriteInfoLabel, row++, 1, 1, 1);

    setLayout(mainLayout);
}

void TotalWidget::updateCpuLabels()
{
    cpuTotalInfoLabel->setText(QString::number(infoManager->getLoad(0), 'd', dataPrecision) + "%");
    for(int i = 0; i < coreCount; ++i)
    {
        float usage = infoManager->getLoad(i + 1);
        coreLabels[i].second->setText(QString::number(usage, 'd', dataPrecision) + "%");
        corePBs[i]->setValue(std::round(usage));
        updatePBStyle(corePBs[i]);
    }
}

void TotalWidget::updatePBStyle(QProgressBar* progressBar)
{
    if(progressBar->value() < 33)
    {
        progressBar->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                   "stop:0 rgb(90,220,90), stop:1 rgb(70,200,70));"
                                   "margin: 2px 1px 1px 2px;}");
    }
    else if(progressBar->value() < 66)
    {
        progressBar->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                   "stop:0 rgb(255,255,0), stop:1 rgb(240,230,140));"
                                   "margin: 2px 1px 1px 2px;}");
    }
    else
    {
        progressBar->setStyleSheet("QProgressBar::chunk {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                   "stop:0 rgb(255,0,0), stop:1 rgb(220,20,60));"
                                   "margin: 2px 1px 1px 2px;}");
    }
}

void TotalWidget::updateMemLabels()
{
    // multiplying by 1024 because /proc/memstat provides data in kilobytes
    memoryTotalInfoLabel->setText(Translator::fitBytes(infoManager->getMemoryTotal() * 1024, dataPrecision));
    memoryAvailPieChart->getPieChart()->clear(true);
    Translator::SizedValue memAvail = Translator::fitBytesToNumber(infoManager->getMemoryAvailable() * 1024, dataPrecision);
    memoryAvailPieChart->getPieChart()->addValue(tr("Available"), memAvail.first, memAvail.second, QBrush(hddAvailColor), true);
    Translator::SizedValue memBusy = Translator::fitBytesToNumber(infoManager->getMemoryBusy() * 1024, dataPrecision);
    memoryAvailPieChart->getPieChart()->addValue(tr("Busy"), memBusy.first, memBusy.second, QBrush(hddBusyColor));
    swapTotalInfoLabel->setText(Translator::fitBytes(infoManager->getSwapTotal() * 1024, dataPrecision));
    swapAvailPieChart->getPieChart()->clear(true);
    Translator::SizedValue swapAvail = Translator::fitBytesToNumber(infoManager->getSwapAvailable() * 1024, dataPrecision);
    swapAvailPieChart->getPieChart()->addValue(tr("Available"), swapAvail.first, swapAvail.second, QBrush(hddAvailColor), true);
    Translator::SizedValue swapBusy = Translator::fitBytesToNumber(infoManager->getSwapBusy() * 1024, dataPrecision);
    swapAvailPieChart->getPieChart()->addValue(tr("Busy"), swapBusy.first, swapBusy.second, QBrush(hddBusyColor));
}

void TotalWidget::updateNetLabels()
{
    networkIncomeInfoLabel->setText(Translator::fitBytes(infoManager->getIncome(), dataPrecision));
    networkOutcomeInfoLabel->setText(Translator::fitBytes(infoManager->getOutcome(), dataPrecision));
}

void TotalWidget::updateHddLabels()
{
    hddReadInfoLabel->setText(Translator::fitBytes(infoManager->getDeviceActivity().bytesRead, dataPrecision));
    hddWriteInfoLabel->setText(Translator::fitBytes(infoManager->getDeviceActivity().bytesWrite, dataPrecision));
}

// settings setters
void TotalWidget::setDataPrecision(int prec)
{
    dataPrecision = prec;
    update();
}

void TotalWidget::setChartGridEnabled(bool) {}

void TotalWidget::setChartMode(Modes) {}

/*
    Tries to continue or start work.
    If it can not, does nothing.
*/
void TotalWidget::start()
{
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...)
    {
        qWarning() << "Total widget is not working";
    }
}

/*
    Private method that updates labels.
*/
void TotalWidget::updater()
{
    if(infoManager->isCpuInfoInitialized())
    {
        updateCpuLabels();
    }
    if(infoManager->isMemInfoInitialized())
    {
        updateMemLabels();
    }
    if(infoManager->isNetInfoInitialized())
    {
        updateNetLabels();
    }
    if(infoManager->isHddInfoInitialized())
    {
        updateHddLabels();
    }
}
