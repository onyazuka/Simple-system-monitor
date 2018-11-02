#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "emulateablewidget.hpp"
#include "../settings.hpp"
#include "../applicationnamespace.hpp"
#include "../utils.hpp"
#include "core/cpuinfo.hpp"
#include "core/meminfo.hpp"
#include "core/netinfo.hpp"

/*--------------------TESTS--------------------*/
class SystemMonitorTest;
/*--------------------/TESTS--------------------*/

/*
    Class representing options.
    Well, it is not emulateable widget, but for convenience it is as emulateable.
*/
class OptionsWidget : public EmulateableWidget
{
    friend class SystemMonitorTest;
    Q_OBJECT
public:
    OptionsWidget(int coresCount, const Settings& settings, QWidget* parent=nullptr);
    const Settings& getSettings() const {return initSettings;}
    void _reset();

private:
    QLabel* cpuStatPathLabel;
    QLineEdit* cpuStatPathLE;
    QLabel* memStatPathLabel;
    QLineEdit* memStatPathLE;
    QLabel* netStatPathLabel;
    QLineEdit* netStatPathLE;
    QLabel* dataPrecisionLabel;
    QSpinBox* dataPrecisionSB;
    QLabel* gridOnOffLabel;
    QCheckBox* gridOnOffCB;
    QLabel* defaultChartsModeLabel;
    QComboBox* defaultChartsModeCB;
    QLabel* languageLabel;
    QComboBox* languageCB;
    QPushButton* defaultPB;
    QPushButton* savePB;
    QPushButton* cancelPB;

    Settings initSettings;
    int cores;

    void updateWidgets(const Settings& settings);
    bool areSystemSettingsChanged() const;
    bool checkStatPaths();

public slots:
    // yes, we don't want any emulation
    void start() {}
    void stop() {}

private slots:
    void resetSettingsToDefault();
    void resetSettings();
    void saveSettings();

signals:
    void settingsChanged();
    void systemSettingsChanged();
};
