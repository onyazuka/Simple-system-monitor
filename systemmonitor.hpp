#pragma once
#include "widgets/totalwidget.hpp"
#include "widgets/cpuwidget.hpp"
#include "widgets/memorywidget.hpp"
#include "widgets/networkwidget.hpp"
#include "widgets/hddwidget.hpp"
#include "widgets/processwidget/processeswidget.hpp"
#include "widgets/optionswidget.hpp"
#include "settings.hpp"

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
    enum Widgets{Total, CPU, Memory, Network, Hdd, Processes, Options};
    typedef QVector<QString> WidgetsIcons;
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
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

    //styling
    static const WidgetsIcons widgetsIcons;
    static const WidgetsIcons widgetsReversedIcons;
    int lastActiveTab;

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
    void changeTabIcon(int index);

private slots:
    void applySettings(const Settings& settings);
    void restart();
};
