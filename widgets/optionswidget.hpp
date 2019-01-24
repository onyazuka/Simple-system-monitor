#pragma once
#include <QtWidgets>
#include "../settings.hpp"
#include "../applicationnamespace.hpp"
#include "../utils.hpp"
#include "processwidget/processeswidget.hpp"

/*--------------------TESTS--------------------*/
class SystemMonitorTest;
/*--------------------/TESTS--------------------*/

/*
    Class representing options.
    Well, it is not emulateable widget, but for convenience it is as emulateable.
*/
class OptionsWidget : public QWidget
{
    friend class SystemMonitorTest;
    Q_OBJECT
public:
    OptionsWidget(const Settings& settings, QWidget* parent=nullptr);
    const Settings& getSettings() const {return initSettings;}
    void _reset();

private:
    void createWidgets();
    void updateWidgetsEnableList(const Settings& _settings);
    void updateProcessesColumnsList(const Settings& _settings);
    void createLayout();
    void makeConnections();

    QScrollArea* systemScrollArea;
    QLabel* languageLabel;
    QComboBox* languageCB;
    QLabel* widgetsEnableLabel;
    QListWidget* widgetsEnableLW;

    QScrollArea* styleScrollArea;
    QLabel* dataPrecisionLabel;
    QSpinBox* dataPrecisionSB;
    QLabel* gridOnOffLabel;
    QCheckBox* gridOnOffCB;
    QLabel* defaultChartsModeLabel;
    QComboBox* defaultChartsModeCB;
    QPushButton* defaultPB;
    QPushButton* savePB;
    QPushButton* cancelPB;

    Settings initSettings;
    int cores;

    void updateWidgets(const Settings& settings);
    bool areEnableWidgetsSettingsChanged() const;
    bool areSystemSettingsChanged() const;

private slots:
    void resetSettingsToDefault();
    void resetSettings();
    void saveSettings();

signals:
    void settingsChanged();
    void systemSettingsChanged();
};
