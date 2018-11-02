#include "widgets/totalwidget.hpp"
#include "widgets/cpuwidget.hpp"
#include "widgets/memorywidget.hpp"
#include "widgets/networkwidget.hpp"
#include "widgets/optionswidget.hpp"
#include "settings.hpp"

/*--------------------TESTS--------------------*/
class SystemMonitorTest : public QObject
{
    Q_OBJECT
private slots:
    void Settings_Test();
};
/*--------------------/TESTS--------------------*/

const QString GlobalStylesheet = QString("QProgressBar::chunk {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                 "stop:0 rgb(90,220,90), stop:1 rgb(70,200,70));"
                                 "margin: 2px 1px 1px 2px;}"
                                 "QProgressBar {text-align: center;}"
                                 "QLabel#%1 {font-size: 11px; font-weight:bold;}"
                                 "QLabel#%2 {font-size: 11px; font-style:italic;}").arg(titleLabelsName, chartDescriptionName);

/*
    Tabs container - main class.
    TabWidgets - used for identifying widget type by tab index
    Also contains widgets for tabs.
    Manages settings and info core classes.
        So only this widget can manage cpuinfo/meminfo/netinfo.
        Other classed can only read them and call update method.
*/
class SystemMonitor : public QWidget
{
    friend class SystemMonitorTest;
    Q_OBJECT
public:
    SystemMonitor(QWidget* parent=nullptr);
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    enum Widgets{Total, CPU, Memory, Network};
    QTabWidget* tabWidget;
    TotalWidget* totalWidget;
    CPUWidget* cpuWidget;
    MemoryWidget* memoryWidget;
    NetworkWidget* networkWidget;
    OptionsWidget* optionsWidget;
    CPUInfo* cpuinfo;
    MemInfo* meminfo;
    NetInfo* netinfo;
    Settings appSettings;

    void applyPalette();
    void saveQSettings();
    void loadQSettings();

public slots:
    void activateWidget(int index);

private slots:
    void applySettings(const Settings& settings);
    void restart();
};
