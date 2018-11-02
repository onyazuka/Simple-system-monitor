#include "systemmonitor.hpp"

SystemMonitor::SystemMonitor(QWidget *parent)
    : QWidget{parent}, cpuinfo{nullptr}, meminfo{nullptr},
      netinfo{nullptr}, appSettings{DefaultSettings}
{
    loadQSettings();

    // trying to init info core class instances.
    // if we can not, we are not printing anything here(it does applySettings metod later)
    // also, doing 3 tries because we can have, for example, 1 invalid cpu stat file, and valid others.
    try
    {
        cpuinfo = new CPUInfo(appSettings.cpuStatPath.toStdString());
    }
    catch (CPUParseError&) {}
    try
    {
        meminfo = new MemInfo(appSettings.memStatPath.toStdString());
    }
    catch (MemoryParseError&) {}
    try
    {
        netinfo = new NetInfo(appSettings.netStatPath.toStdString());
    }
    catch (NetworkParseError&) {}

    // widgets
    tabWidget = new QTabWidget;
    totalWidget = new TotalWidget(cpuinfo, meminfo, netinfo);
    cpuWidget = new CPUWidget(cpuinfo);
    memoryWidget = new MemoryWidget(meminfo);
    networkWidget = new NetworkWidget(netinfo);
    optionsWidget = new OptionsWidget(9, appSettings);
    applyPalette();
    applySettings(appSettings);

    tabWidget->addTab(totalWidget, tr("Total"));
    tabWidget->addTab(cpuWidget, tr("CPU"));
    tabWidget->addTab(memoryWidget, tr("Memory"));
    tabWidget->addTab(networkWidget, tr("Network"));
    tabWidget->addTab(optionsWidget, tr("Options"));

    QVBoxLayout* vbl = new QVBoxLayout;
    vbl->addWidget(tabWidget);
    setLayout(vbl);

    setWindowTitle(tr("Simple system monitor"));

    //connecting
    connect(tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(activateWidget(int)));
    connect(optionsWidget, &OptionsWidget::settingsChanged, this, [this](){applySettings(optionsWidget->getSettings());});
    connect(optionsWidget, SIGNAL(systemSettingsChanged()), this, SLOT(restart()));
    activateWidget(Widgets::Total);
}

void SystemMonitor::closeEvent(QCloseEvent *event)
{
    saveQSettings();
    QWidget::closeEvent(event);
}

// configuring charts palettes
void SystemMonitor::applyPalette()
{
    QVector<QColor> chartsPalette;
    chartsPalette << QColor("red") << QColor("blue") << QColor("black") << QColor("brown")
                  << QColor("purple") << QColor("cyan") << QColor("magenta") << QColor("green");
    for(int i = 0; i < cpuWidget->getCpuChartsCount(); ++i)
    {
        cpuWidget->getCpuChart(i).setDefaultPalette(chartsPalette);
    }
    memoryWidget->getMemoryUsageChart().setDefaultPalette(chartsPalette);
    memoryWidget->getSwapUsageChart().setDefaultPalette(chartsPalette);
    networkWidget->getNetworkIncomeChart().setDefaultPalette(chartsPalette);
    networkWidget->getNetworkOutcomeChart().setDefaultPalette(chartsPalette);
}

void SystemMonitor::saveQSettings()
{
    QSettings settings;
    settings.setValue("CpuStat", appSettings.cpuStatPath);
    settings.setValue("MemStat", appSettings.memStatPath);
    settings.setValue("NetStat", appSettings.netStatPath);
    settings.setValue("DataPrec", appSettings.dataPrecision);
    settings.setValue("GridOn", appSettings.gridOn);
    settings.setValue("DefMode", (int)appSettings.defaultChartsMode);
    settings.setValue("Language", (int)appSettings.language);
}

void SystemMonitor::loadQSettings()
{
    QSettings settings;
    if(settings.contains("CpuStat"))
    {
        appSettings.cpuStatPath = settings.value("CpuStat").value<QString>();
    }
    if(settings.contains("MemStat"))
    {
        appSettings.memStatPath = settings.value("MemStat").value<QString>();
    }
    if(settings.contains("NetStat"))
    {
        appSettings.netStatPath = settings.value("NetStat").value<QString>();
    }
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
}

/*
    Called when tab changed - activates current tab's widget
*/
void SystemMonitor::activateWidget(int index)
{

    for(int i = 0; i < tabWidget->count(); ++i)
    {
        if(i == index) continue;
        qobject_cast<EmulateableWidget*>(tabWidget->widget(index))
                ->stop();
    }
    qobject_cast<EmulateableWidget*>(tabWidget->widget(index))
            ->start();
}

/*
    Only this main widgets can update other widgets.
    TODO: it will be better to not update all settings, but only changed settings!
*/
void SystemMonitor::applySettings(const Settings& settings)
{

    appSettings = settings;

    try     // cpu stat path
    {

        if(cpuinfo) cpuinfo->setCpuStatFileName(appSettings.cpuStatPath.toStdString());
        else
        {
            cpuinfo = new CPUInfo(appSettings.cpuStatPath.toStdString());
            update();
        }
        cpuinfo->update();
        cpuWidget->restartCharts();
    }
    catch(CPUParseError& err)
    {
        cpuWidget->stopCharts();
        totalWidget->stop();
        cpuWidget->stop();
        QMessageBox::warning(this, tr("Error"), tr("Incorrect CPU stat file"));
    }

    try     // mem info path
    {
        if(meminfo) meminfo->setMemInfoFileName(appSettings.memStatPath.toStdString());
        else
        {
            meminfo = new MemInfo(appSettings.memStatPath.toStdString());
            update();
        }
        meminfo->update();
        memoryWidget->restartCharts();
    }
    catch(MemoryParseError& err)
    {
        memoryWidget->stopCharts();
        totalWidget->stop();
        memoryWidget->stop();
        QMessageBox::warning(this, tr("Error"), tr("Incorrect memory info file"));
    }

    try     // net stat path
    {
        if(netinfo) netinfo->setNetInfoFileName(appSettings.netStatPath.toStdString());
        else
        {
            netinfo = new NetInfo(appSettings.netStatPath.toStdString());
            update();
        }
        netinfo->update();
        networkWidget->restartCharts();
    }
    catch(NetworkParseError& err)
    {
        networkWidget->stopCharts();
        totalWidget->stop();
        networkWidget->stop();
        QMessageBox::warning(this, tr("Error"), tr("Incorrect network info file"));
    }

    totalWidget->setDataPrecision(appSettings.dataPrecision);
    memoryWidget->setDataPrecision(appSettings.dataPrecision);
    networkWidget->setDataPrecision(appSettings.dataPrecision);
    networkWidget->getNetworkIncomeChart().setDataPrecision(appSettings.dataPrecision);
    networkWidget->getNetworkOutcomeChart().setDataPrecision(appSettings.dataPrecision);
    memoryWidget->getMemoryUsageChart().setEnableGrid(appSettings.gridOn);
    memoryWidget->getSwapUsageChart().setEnableGrid(appSettings.gridOn);
    networkWidget->getNetworkIncomeChart().setEnableGrid(appSettings.gridOn);
    networkWidget->getNetworkOutcomeChart().setEnableGrid(appSettings.gridOn);

    for(int i = 0; i < cpuWidget->getCpuChartsCount(); ++i)
    {
        cpuWidget->getCpuChart(i).setMode(appSettings.defaultChartsMode);
        cpuWidget->getCpuChart(i).setEnableGrid(appSettings.gridOn);
    }

    memoryWidget->getMemoryUsageChart().setMode(appSettings.defaultChartsMode);
    memoryWidget->getSwapUsageChart().setMode(appSettings.defaultChartsMode);
    networkWidget->getNetworkIncomeChart().setMode(appSettings.defaultChartsMode);
    networkWidget->getNetworkOutcomeChart().setMode(appSettings.defaultChartsMode);
    // language is not set, it is applied as application's language on restart

    totalWidget->update();
    cpuWidget->update();
    memoryWidget->update();
    networkWidget->update();
}

/*
    Restarts, not closes application.
*/
void SystemMonitor::restart()
{
    saveQSettings();
    QApplication::exit(RestartExitCode);
}
