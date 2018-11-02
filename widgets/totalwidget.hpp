#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "emulateablewidget.hpp"
#include "core/cpuinfo.hpp"
#include "core/meminfo.hpp"
#include "core/netinfo.hpp"
#include "utils.hpp"
#include "../applicationnamespace.hpp"

const QString MAIN_DLG_STYLESHEET = "QLabel#Title {color: red;}";

/*
    User can change:
        update time,
        data precision
    User SHOULD call start method to start working.
*/
class TotalWidget : public EmulateableWidget
{
    Q_OBJECT
    typedef QPair<QLabel*, QLabel*> CoreLabelsPair;
    typedef QVector<CoreLabelsPair> CoreLabels;
    typedef QVector<QProgressBar*> CoreProgressBars;
public:
    TotalWidget(CPUInfo* _cpuInfo, MemInfo* _meminfo, NetInfo* _netInfo, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}
    inline void setDataPrecision(int precision) {dataPrecision = precision;}

private:
    CPUInfo* cpuinfo;
    MemInfo* meminfo;
    NetInfo* netInfo;
    int coreCount;

    //settings
    int dataPrecision;

    //widgets
    QLabel* memoryLabel;
    QLabel* memoryTotalLabel;
    QLabel* memoryTotalInfoLabel;
    QLabel* memoryAvailLabel;
    QLabel* memoryAvailInfoLabel;
    QProgressBar* memoryAvailPB;
    QLabel* memoryUsageLabel;
    QLabel* memoryUsageInfoLabel;
    QProgressBar* memoryUsagePB;

    QLabel* swapLabel;
    QLabel* swapTotalLabel;
    QLabel* swapTotalInfoLabel;
    QLabel* swapAvailLabel;
    QLabel* swapAvailInfoLabel;
    QProgressBar* swapAvailPB;

    QLabel* cpuLabel;
    QLabel* cpuTotalLabel;
    QLabel* cpuTotalInfoLabel;
    CoreLabels coreLabels;
    CoreProgressBars corePBs;

    QLabel* networkLabel;
    QLabel* networkIncomeLabel;
    QLabel* networkIncomeInfoLabel;
    QLabel* networkOutcomeLabel;
    QLabel* networkOutcomeInfoLabel;

protected slots:
    void start();
    virtual void updater();
};
