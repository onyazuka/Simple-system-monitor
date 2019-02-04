#pragma once
#include "emulateablewidget.hpp"
#include "../infomanager.hpp"
#include "utils.hpp"
#include "configurablewidget.hpp"
#include "charts/PieChart/labeled_piechart.hpp"

const QString MAIN_DLG_STYLESHEET = "QLabel#Title {color: red;}";

/*
    User can change:
        update time,
        data precision
    User SHOULD call start method to start working.
*/
class TotalWidget : public EmulateableWidget, public ConfigurableWidget
{
    Q_OBJECT
    typedef QPair<QLabel*, QLabel*> CoreLabelsPair;
    typedef QVector<CoreLabelsPair> CoreLabels;
    typedef QVector<QProgressBar*> CoreProgressBars;
public:
    TotalWidget(InfoManager::pointer _infoManager, QWidget* parent=nullptr);

    //getters/setters
    inline int getDataPrecision() const {return dataPrecision;}

private:
    void createWidgets();
    void createLayout();

    void updateCpuLabels();
    void updatePBStyle(QProgressBar* progressBar);
    void updateMemLabels();
    void updateNetLabels();
    void updateHddLabels();

    // settings setters
    void setDataPrecision(int prec);
    void setChartGridEnabled(bool on);
    void setChartMode(Modes mode);

    InfoManager::pointer infoManager;
    int coreCount;

    //settings
    int dataPrecision;

    //widgets
    QLabel* memoryLabel;
    QLabel* memoryTotalLabel;
    QLabel* memoryTotalInfoLabel;
    LabeledPiechart* memoryAvailPieChart;

    QLabel* swapLabel;
    QLabel* swapTotalLabel;
    QLabel* swapTotalInfoLabel;
    LabeledPiechart* swapAvailPieChart;

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

    QLabel* hddLabel;
    QLabel* hddReadLabel;
    QLabel* hddReadInfoLabel;
    QLabel* hddWriteLabel;
    QLabel* hddWriteInfoLabel;

    QFrame* memorySplitter;
    QFrame* swapSplitter;
    QFrame* cpuSplitter;
    QFrame* netSplitter;

protected slots:
    void start();
    virtual void updater();
};
