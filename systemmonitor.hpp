#pragma once
#include "widgets/totalwidget.hpp"
#include "widgets/cpuwidget.hpp"
#include "widgets/memorywidget.hpp"
#include "widgets/networkwidget.hpp"
#include "widgets/hddwidget.hpp"
#include "widgets/processwidget/processeswidget.hpp"
#include "widgets/optionswidget.hpp"
#include "settings.hpp"

const QString GlobalStylesheet = QString("QProgressBar::chunk {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                 "stop:0 rgb(90,220,90), stop:1 rgb(70,200,70));"
                                 "margin: 2px 1px 1px 2px;}"
                                 "QProgressBar {text-align: center;}"
                                 "QLabel#%1 {font-size: 11px; font-weight:bold;}"
                                 "QLabel#%2 {font-size: 11px; font-style:italic;}"
                                 "QLabel#%3 {font-size: 11px; font-style:italic;}"
                                 "QLabel#%4 {font-size: 11px; font-style:italic;}").arg(titleLabelsName, chartDescriptionName, infoLabelsName, propertyLabelsName);

/*
    Tabs container - main class.
    TabWidgets - used for identifying widget type by tab index
    Also contains widgets for tabs.
    Manages settings and info core classes(by InfoManager creation and passing to others).
*/
class SystemMonitor : public QWidget
{
    friend class SystemMonitorTest;
    Q_OBJECT
public:
    SystemMonitor(QWidget* parent=nullptr);
    const Settings& getAppSettings() const { return appSettings; }
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    enum Widgets{Total, CPU, Memory, Network, Hdd, Processes, Options};
    QTabWidget* tabWidget;
    TotalWidget* totalWidget;
    CPUWidget* cpuWidget;
    MemoryWidget* memoryWidget;
    NetworkWidget* networkWidget;
    HddWidget* hddWidget;
    ProcessesWidget* procWidget;
    OptionsWidget* optionsWidget;
    InfoManager::pointer infoManager;
    Settings appSettings;

    void createInfoManager(bool initCpu = true, bool initMem = true, bool initNet = true, bool initHdd = true, bool initProc = true);
    void createWidgets();
    void createSettingsWidget();
    void createLayout();
    void createConnections();
    void applyPalette();
    void activateWidgets();
    void saveQSettings(const Settings& _settings);
    void loadQSettings();

public slots:
    void activateWidgetAndStopOthers(int index);
    void widgetsStarterStopper(int index);

private slots:
    void applySettings(const Settings& settings);
    void restart();
};
