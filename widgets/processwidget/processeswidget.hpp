#pragma once
#include "../emulateablewidget.hpp"
#include "../configurablewidget.hpp"
#include "../../infomanager.hpp"
#include "utils.hpp"
#include "processwidgetmodel.hpp"
#include "system/system.hpp"

class ProcessesWidget : public EmulateableWidget, public ConfigurableWidget
{
    Q_OBJECT
public:
    ProcessesWidget(InfoManager::pointer _infoManager, QWidget* parent = nullptr);
    inline int processes() const { return processesModel->getProcessActivities().size(); }
    ProcessActivities getActivitiesOfPID(PID pid);

private:
    void createWidgets();
    void createLayout();
    void makeConnections();

    // configurable interface
    void setDataPrecision(int prec);
    void setChartGridEnabled(bool on);
    void setChartMode(Modes mode);

    InfoManager::pointer infoManager;
    ProcessWidgetModel* processesModel;
    ProcessWidgetProxyModel* sortProxyModel;
    QTableView* processesView;

public slots:
    void start();

protected slots:
    virtual void updater();
    void showProcessContextMenu(QPoint point, PID pid);
    void killProcess(PID pid);
    void terminateProcess(PID pid);
};

