#include "hddwidget.hpp"

HddWidget::HddWidget(InfoManager::pointer _infoManager, QWidget* parent)
    : EmulateableWidget{parent}, dataPrecision{2}, infoManager{_infoManager}
{
    setUpdateInterval(500);
    createWidgets();
    createAndUpdateSpacesWidgets();
    createLayout();
    updateSpacesLayout();
    createUpdaterFunctions();
}

void HddWidget::createWidgets()
{
    totalHddReadInfoLabel = new QLabel;
    totalHddReadInfoLabel->setObjectName(chartDescriptionName);
    totalHddWriteInfoLabel = new QLabel;
    totalHddWriteInfoLabel->setObjectName(chartDescriptionName);
    spaceLabel = new QLabel(tr("Disk space information: "));
    spaceLabel->setObjectName(titleLabelsName);
    activitiesLabel = new QLabel(tr("Disk activities information: "));
    activitiesLabel->setObjectName(titleLabelsName);
    spaceActivitiesSplitter = new QFrame();
    spaceActivitiesSplitter->setFrameShape(QFrame::HLine);
    spaceActivitiesSplitter->setFrameShadow(QFrame::Sunken);
    hddReadChart = new HddChart(1);
    hddWriteChart = new HddChart(1);
}

void HddWidget::createAndUpdateSpacesWidgets()
{
    deviceLabels.clear();
    devicePieCharts.clear();
    const auto& deviceInfoMap = infoManager->getDeviceInfoMap();
    for(auto device: deviceInfoMap)
    {
        QLabel* newLabel = new QLabel(QString::fromStdString(device.second.devicePath));
        deviceLabels.push_back(newLabel);
        newLabel->setObjectName(infoLabelsName);
        LabeledPiechart* newLabeledPieChart = new LabeledPiechart(LabeledPiechart::Direction::Right);
        newLabeledPieChart->getPieChart()->setFixedSize(100, 100);
        newLabeledPieChart->getPieChart()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        Translator::SizedValue hddFree = Translator::fitBytesToNumber(device.second.freeSpace, dataPrecision);
        newLabeledPieChart->getPieChart()->addValue(tr("Free"), hddFree.first, hddFree.second, QBrush(hddAvailColor), true);
        Translator::SizedValue hddBusy = Translator::fitBytesToNumber(device.second.totalSpace - device.second.freeSpace, dataPrecision);
        newLabeledPieChart->getPieChart()->addValue(tr("Busy"), hddBusy.first, hddBusy.second, QBrush(hddBusyColor));
        devicePieCharts.push_back(newLabeledPieChart);
    }
}

void HddWidget::createLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    deviceSpacesLayout = new QGridLayout;
    mainLayout->addWidget(spaceLabel);
    mainLayout->addLayout(deviceSpacesLayout);
    mainLayout->addWidget(spaceActivitiesSplitter);
    mainLayout->addWidget(activitiesLabel);
    mainLayout->addWidget(totalHddReadInfoLabel);
    mainLayout->addWidget(hddReadChart);
    mainLayout->addWidget(totalHddWriteInfoLabel);
    mainLayout->addWidget(hddWriteChart);
    setLayout(mainLayout);
}

void HddWidget::updateSpacesLayout()
{
    clearLayout(deviceSpacesLayout);
    int row = 0;
    for(int i = 0; i < deviceLabels.size(); ++i)
    {
        QVBoxLayout* deviceSpacesSublayout = new QVBoxLayout;
        deviceSpacesSublayout->addWidget(deviceLabels[i], 1, Qt::AlignCenter);
        deviceSpacesSublayout->addWidget(devicePieCharts[i], 5, Qt::AlignCenter);
        deviceSpacesLayout->addLayout(deviceSpacesSublayout, row, i % 2, 1, 1);
        if(i % 2 == 1)
        {
            ++row;
        }
    }
}

void HddWidget::createUpdaterFunctions()
{
    HddReadUpdater hru(infoManager);
    HddWriteUpdater hwu(infoManager);
    hddReadChart->setUpdaterFunction(hru);
    hddWriteChart->setUpdaterFunction(hwu);
}

void HddWidget::clearLayout(QLayout* layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget())
        {
            delete item->widget();
        }
    }
}

// settings setters
void HddWidget::setDataPrecision(int prec)
{
    dataPrecision = prec;
    hddReadChart->setDataPrecision(dataPrecision);
    hddWriteChart->setDataPrecision(dataPrecision);
    update();
}

void HddWidget::setChartGridEnabled(bool on)
{
    getHddReadChart().setEnableGrid(on);
    getHddWriteChart().setEnableGrid(on);
    update();
}

void HddWidget::setChartMode(Modes mode)
{
    getHddReadChart().setMode(mode);
    getHddWriteChart().setMode(mode);
    update();
}

void HddWidget::stopCharts()
{
    hddReadChart->stop();
    hddWriteChart->stop();
}

void HddWidget::restartCharts()
{
    hddReadChart->start();
    hddWriteChart->start();
}

/*
    Tries to continue or start work.
    If it can not, does nothing.
*/
void HddWidget::start()
{
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...)
    {
        qWarning() << "Hdd widget is not working";
    }
}

void HddWidget::updater()
{
    createAndUpdateSpacesWidgets();
    updateSpacesLayout();
    totalHddReadInfoLabel->setText(tr("Total read: ") + Translator::fitBytes(infoManager->getDeviceActivity().bytesRead, dataPrecision));
    totalHddWriteInfoLabel->setText(tr("Total write: ") + Translator::fitBytes(infoManager->getDeviceActivity().bytesWrite, dataPrecision));
}
