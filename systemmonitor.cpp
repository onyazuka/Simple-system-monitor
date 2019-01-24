#include "systemmonitor.hpp"

SystemMonitor::SystemMonitor(QWidget *parent)
    : QWidget{parent}, infoManager{nullptr}, appSettings{DefaultSettings}
{
    loadQSettings();
    createInfoManager(appSettings.enableCpu, appSettings.enableMem, appSettings.enableNet, appSettings.enableHdd, appSettings.enableProc);
    // all widgets except settings
    createWidgets();
    applySettings(appSettings);
    createSettingsWidget();
    createLayout();
    applyPalette();
    setWindowTitle(tr("Simple system monitor"));
    createConnections();
    activateWidgets();
}

void SystemMonitor::closeEvent(QCloseEvent *event)
{
    saveQSettings(appSettings);
    infoManager.reset();
    QWidget::closeEvent(event);
}

/*
    All except ProcessesWidget
*/
void SystemMonitor::activateWidgets()
{
    for(int i = 0; i < tabWidget->count(); ++i)
    {
        EmulateableWidget* thisWidget = qobject_cast<EmulateableWidget*>(tabWidget->widget(i));
        if(thisWidget == nullptr)
        {
            continue;
        }
        if(qobject_cast<ProcessesWidget*>(tabWidget->widget(i)) != nullptr)
        {
            continue;
        }
        thisWidget->start();
    }
}

/*
   _settings argument is needed for testing purposes
*/
void SystemMonitor::saveQSettings(const Settings& _settings)
{
    QSettings settings;
    settings.setValue("DataPrec", _settings.dataPrecision);
    settings.setValue("GridOn", _settings.gridOn);
    settings.setValue("DefMode", (int)_settings.defaultChartsMode);
    settings.setValue("Language", (int)_settings.language);
    settings.setValue("cpuOn", _settings.enableCpu);
    settings.setValue("memOn", _settings.enableMem);
    settings.setValue("netOn", _settings.enableNet);
    settings.setValue("hddOn", _settings.enableHdd);
    settings.setValue("procOn", _settings.enableProc);
}

void SystemMonitor::loadQSettings()
{
    QSettings settings;
    if(settings.contains("DataPrec"))
    {
        appSettings.dataPrecision = settings.value("DataPrec").value<int>();
    }
    if(settings.contains("GridOn"))
    {
        appSettings.gridOn = settings.value("GridOn").value<bool>();
    }
    if(settings.contains("DefMode"))
    {
        appSettings.defaultChartsMode = (Modes)settings.value("DefMode").value<int>();
    }
    if(settings.contains("CpuStat"))
    {
        appSettings.language = (Languages)settings.value("Language").value<int>();
    }
    if(settings.contains("cpuOn"))
    {
        appSettings.enableCpu = settings.value("cpuOn").value<bool>();
    }
    if(settings.contains("memOn"))
    {
        appSettings.enableMem = settings.value("memOn").value<bool>();
    }
    if(settings.contains("netOn"))
    {
        appSettings.enableNet = settings.value("netOn").value<bool>();
    }
    if(settings.contains("hddOn"))
    {
        appSettings.enableHdd = settings.value("hddOn").value<bool>();
    }
    if(settings.contains("procOn"))
    {
        appSettings.enableProc = settings.value("procOn").value<bool>();
    }
}

// NOT ACTUALLY CALLED
void SystemMonitor::activateWidgetAndStopOthers(int index)
{

    // if we want to stop everything else
    for(int i = 0; i < tabWidget->count(); ++i)
    {
        if(i == index) continue;
        if(qobject_cast<EmulateableWidget*>(tabWidget->widget(i)) == nullptr)
        {
            continue;
        }
        qobject_cast<EmulateableWidget*>(tabWidget->widget(i))
                ->stop();
    }

    if(( qobject_cast<EmulateableWidget*>(tabWidget->widget(index)) == nullptr))
    {
        return;
    }
    qobject_cast<EmulateableWidget*>(tabWidget->widget(index))->start();
}

/*
    'index' - index of clicked tab
    here stored all starting/stopping logic for tab widgets
*/
void SystemMonitor::widgetsStarterStopper(int)
{
    // as most consuming widget, procWidget is to be stopped when not shown
    ProcessesWidget* pw = qobject_cast<ProcessesWidget*>(tabWidget->currentWidget());
    if(pw != nullptr && infoManager->isProcInfoInitialized())
    {
        procWidget->start();
    }
    else if(infoManager->isProcInfoInitialized())
    {
        procWidget->stop();
    }
}

void SystemMonitor::createInfoManager(bool initCpu, bool initMem, bool initNet, bool initHdd, bool initProc)
{

    infoManager = InfoManager::pointer(new InfoManager(initCpu, initMem, initNet, initHdd, initProc));

    // if we do not asked cpu initialization and it was, of course, not initialized, we do not want to error be showed
    if (initCpu && !infoManager->isCpuInfoInitialized())
    {
        QString errorInfo = tr("Can not get cpu info.");
        QMessageBox::warning(this, tr("Error"), errorInfo);
    }
    else if(infoManager->isCpuInfoInitialized())
    {
        infoManager->setCpuUpdateInterval(500);
    }

    if(initMem && !infoManager->isMemInfoInitialized())
    {
        QString errorInfo = tr("Can not get memory info.");
        QMessageBox::warning(this, tr("Error"), errorInfo);
    }
    else if(infoManager->isMemInfoInitialized())
    {
        infoManager->setMemUpdateInterval(500);
    }

    if(initNet && !infoManager->isNetInfoInitialized())
    {
        QString errorInfo = tr("Can not get network info.");
        QMessageBox::warning(this, tr("Error"), errorInfo);
    }
    else if(infoManager->isNetInfoInitialized())
    {
        infoManager->setNetUpdateInterval(500);
    }

    if(initHdd && !infoManager->isHddInfoInitialized())
    {
        QString errorInfo = tr("Can not get hdd info.");
        QMessageBox::warning(this, tr("Error"), errorInfo);
    }
    else if(infoManager->isHddInfoInitialized())
    {
        infoManager->setHddUpdateInterval(500);
    }

    if(initProc && !infoManager->isProcInfoInitialized())
    {
        QString errorInfo = tr("Can not get processes info.");
        QMessageBox::warning(this, tr("Error"), errorInfo);
    }
    else if(infoManager->isProcInfoInitialized())
    {
        infoManager->setProcUpdateInterval(500);
    }
}

/*
    creates all widgets except settings widget
*/
void SystemMonitor::createWidgets()
{
    tabWidget = new QTabWidget;
    // total widget is created anyways
    totalWidget = new TotalWidget(infoManager);
    tabWidget->addTab(totalWidget, tr("Total"));
    if(infoManager->isCpuInfoInitialized())
    {
        cpuWidget = new CPUWidget(infoManager);
        tabWidget->addTab(cpuWidget, tr("CPU"));
    }
    if(infoManager->isMemInfoInitialized())
    {
        memoryWidget = new MemoryWidget(infoManager);
        tabWidget->addTab(memoryWidget, tr("Memory"));
    }
    if(infoManager->isNetInfoInitialized())
    {
        networkWidget = new NetworkWidget(infoManager);
        tabWidget->addTab(networkWidget, tr("Network"));
    }
    if(infoManager->isHddInfoInitialized())
    {
        hddWidget = new HddWidget(infoManager);
        tabWidget->addTab(hddWidget, tr("HDD"));
    }
    if(infoManager->isProcInfoInitialized())
    {
        procWidget = new ProcessesWidget(infoManager);
        tabWidget->addTab(procWidget, tr("Processes"));
    }
}

void SystemMonitor::createSettingsWidget()
{
    optionsWidget = new OptionsWidget(appSettings);
    tabWidget->addTab(optionsWidget, tr("Options"));
}

void SystemMonitor::createLayout()
{
    QVBoxLayout* vbl = new QVBoxLayout;
    vbl->addWidget(tabWidget);
    setLayout(vbl);
}

void SystemMonitor::createConnections()
{
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(widgetsStarterStopper(int)));
    connect(optionsWidget, &OptionsWidget::settingsChanged, this, [this](){applySettings(optionsWidget->getSettings());});
    connect(optionsWidget, SIGNAL(systemSettingsChanged()), this, SLOT(restart()));
}

// configuring charts palettes
// NOT USED NOW
void SystemMonitor::applyPalette()
{
    QVector<QColor> chartsPalette;
    chartsPalette << QColor("red") << QColor("blue") << QColor("black") << QColor("brown")
                  << QColor("purple") << QColor("cyan") << QColor("magenta") << QColor("green");
    if(infoManager->isCpuInfoInitialized())
    {
        for(int i = 0; i < cpuWidget->getCpuChartsCount(); ++i)
        {
            cpuWidget->getCpuChart(i).setDefaultPalette(chartsPalette);
        }
    }
    if(infoManager->isMemInfoInitialized())
    {
        memoryWidget->getMemoryUsageChart().setDefaultPalette(chartsPalette);
        memoryWidget->getSwapUsageChart().setDefaultPalette(chartsPalette);
    }
    if(infoManager->isNetInfoInitialized())
    {
        networkWidget->getNetworkIncomeChart().setDefaultPalette(chartsPalette);
        networkWidget->getNetworkOutcomeChart().setDefaultPalette(chartsPalette);
    }
}

/*
    Templated method setSettings(settings) of each instance of ConfigurableWidget class is called.
*/
void SystemMonitor::applySettings(const Settings& settings)
{
    appSettings = settings;
    for(int i = 0; i < tabWidget->count(); ++i)
    {
        ConfigurableWidget* thisWidget = dynamic_cast<ConfigurableWidget*>(tabWidget->widget(i));
        if(thisWidget == nullptr)
        {
            continue;
        }
        thisWidget->setSettings(settings);
     }
}

/*
    Restarts, not closes application.
*/
void SystemMonitor::restart()
{
    saveQSettings(appSettings);
    QApplication::exit(RestartExitCode);
}
