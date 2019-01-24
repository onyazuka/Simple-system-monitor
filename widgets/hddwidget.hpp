#pragma once
#include "emulateablewidget.hpp"
#include "../infomanager.hpp"
#include "../charts/networkchart.hpp"
#include "utils.hpp"
#include "configurablewidget.hpp"
#include "charts/PieChart/labeled_piechart.hpp"

/*-------------------------UPDATERS---------------------------*/

class HddReadUpdater
{
public:
    HddReadUpdater() {}
    HddReadUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager}, lastRead{_infoManager->getDeviceActivity().bytesRead} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        std::vector<double> res;
        uint64_t thisRead = infoManager->getDeviceActivity().bytesRead;
        res.push_back(thisRead - lastRead);
        lastRead = thisRead;
        return res;
    }
private:
    InfoManager::pointer infoManager;
    uint64_t lastRead;
};

class HddWriteUpdater
{
public:
    HddWriteUpdater() {}
    HddWriteUpdater(InfoManager::pointer _infoManager)
        : infoManager{_infoManager}, lastWrite{_infoManager->getDeviceActivity().bytesWrite} {}
    // updates only if update flag passed
    std::vector<double> operator()()
    {
        std::vector<double> res;
        uint64_t thisWrite = infoManager->getDeviceActivity().bytesWrite;
        res.push_back(thisWrite - lastWrite);
        lastWrite = thisWrite;
        return res;
    }
private:
    InfoManager::pointer infoManager;
    uint64_t lastWrite;
};

/*-------------------------/UPDATERS---------------------------*/

class HddWidget : public EmulateableWidget, public ConfigurableWidget
{
    Q_OBJECT
    typedef NetworkChart HddChart;
    typedef QVector<QLabel*> DeviceLabels;
    typedef QVector<LabeledPiechart*> DevicePieCharts;
public:
    HddWidget(InfoManager::pointer _infoManager, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline HddChart& getHddReadChart() const { return *hddReadChart; }
    inline HddChart& getHddWriteChart() const { return *hddWriteChart; }

private:
    void createWidgets();
    void createAndUpdateSpacesWidgets();
    void createLayout();
    void updateSpacesLayout();
    void createUpdaterFunctions();
    void clearLayout(QLayout* layout);

    // settings setters
    void setDataPrecision(int prec);
    void setChartGridEnabled(bool on);
    void setChartMode(Modes mode);

    //settings
    int dataPrecision;

    //widgets
    InfoManager::pointer infoManager;
    QLabel* spaceLabel;
    QLabel* activitiesLabel;
    QFrame* spaceActivitiesSplitter;
    QLabel* totalHddReadInfoLabel;
    QLabel* totalHddWriteInfoLabel;
    HddChart* hddReadChart;
    HddChart* hddWriteChart;
    DeviceLabels deviceLabels;
    DevicePieCharts devicePieCharts;
    QGridLayout* deviceSpacesLayout;

// as charts are Emulateable by itselves, and we don't want them to stop when changing tabs,
// we need to provide our own methods for starting/stopping them
public slots:
    void stopCharts();
    void restartCharts();
    void start();

protected slots:
    virtual void updater();
};
