#include "optionswidget.hpp"

/*
    Adding layout to scroll area memo:
        1. Create scroll area;
        2. Create container widget for layout;
        3. Create layout on this widget;
        4. Set this widget to scroll area;
        5. Add scroll area to main layout.
*/
OptionsWidget::OptionsWidget(int coresCount, const Settings& settings, QWidget *parent)
    : EmulateableWidget{parent}, initSettings{settings}, cores{coresCount}
{
    // widgets
    QScrollArea* systemScrollArea = new QScrollArea(this);
    QScrollArea* styleScrollArea = new QScrollArea(this);

    cpuStatPathLabel = new QLabel(tr("CPU stat path: "));
    cpuStatPathLE = new QLineEdit;
    cpuStatPathLabel->setBuddy(cpuStatPathLE);
    cpuStatPathLabel->setObjectName(propertyLabelsName);

    memStatPathLabel = new QLabel(tr("Memory stat path: "));
    memStatPathLE = new QLineEdit;
    memStatPathLabel->setBuddy(memStatPathLabel);
    memStatPathLabel->setObjectName(propertyLabelsName);

    netStatPathLabel = new QLabel(tr("Network stat path: "));
    netStatPathLE = new QLineEdit;
    netStatPathLabel->setBuddy(netStatPathLabel);
    netStatPathLabel->setObjectName(propertyLabelsName);

    languageLabel = new QLabel(tr("Language: "));
    languageCB = new QComboBox;
    languageCB->addItem(Translator::languageToString(Languages::English), Languages::English);
    languageCB->addItem(Translator::languageToString(Languages::Russian), Languages::Russian);
    languageLabel->setBuddy(languageCB);
    languageLabel->setObjectName(propertyLabelsName);

    dataPrecisionLabel = new QLabel(tr("Data precision(digits): "));
    dataPrecisionSB = new QSpinBox;
    dataPrecisionLabel->setBuddy(dataPrecisionSB);
    dataPrecisionSB->setRange(0,3);
    dataPrecisionLabel->setObjectName(propertyLabelsName);

    gridOnOffLabel = new QLabel(tr("Grid on: "));
    gridOnOffCB = new QCheckBox;
    gridOnOffLabel->setBuddy(gridOnOffCB);
    gridOnOffCB->setObjectName(propertyLabelsName);

    defaultChartsModeLabel = new QLabel(tr("Default charts mode: "));
    defaultChartsModeCB = new QComboBox;
    defaultChartsModeCB->addItem(Translator::chartModeToString(Modes::Minute), Modes::Minute);
    defaultChartsModeCB->addItem(Translator::chartModeToString(Modes::TenMinutes), Modes::TenMinutes);
    defaultChartsModeCB->addItem(Translator::chartModeToString(Modes::Hour), Modes::Hour);
    defaultChartsModeLabel->setBuddy(defaultChartsModeCB);
    defaultChartsModeLabel->setObjectName(propertyLabelsName);

    defaultPB = new QPushButton(tr("Reset"));
    savePB = new QPushButton(tr("Save"));
    cancelPB = new QPushButton(tr("Cancel"));

    updateWidgets(settings);

    // layouting

    QWidget* systemScrollWidget = new QWidget;
    QGridLayout* systemLayout = new QGridLayout(systemScrollWidget);
    QWidget* styleScrollWidget = new QWidget;
    QGridLayout* styleLayout = new QGridLayout(styleScrollWidget);

    systemLayout->addWidget(cpuStatPathLabel, 0, 0, 1, 1);
    systemLayout->addWidget(cpuStatPathLE, 0, 1, 1, 1);
    systemLayout->addWidget(memStatPathLabel, 1, 0, 1, 1);
    systemLayout->addWidget(memStatPathLE, 1, 1, 1, 1);
    systemLayout->addWidget(netStatPathLabel, 2, 0, 1, 1);
    systemLayout->addWidget(netStatPathLE, 2, 1, 1, 1);
    systemLayout->addWidget(languageLabel, 3, 0, 1, 1);
    systemLayout->addWidget(languageCB, 3, 1, 1, 1);

    styleLayout->addWidget(dataPrecisionLabel, 0, 0, 1, 1);
    styleLayout->addWidget(dataPrecisionSB, 0, 1, 1, 1);
    styleLayout->addWidget(gridOnOffLabel, 1, 0, 1, 1);
    styleLayout->addWidget(gridOnOffCB, 1, 1, 1, 1);
    styleLayout->addWidget(defaultChartsModeLabel, 2, 0, 1, 1);
    styleLayout->addWidget(defaultChartsModeCB, 2, 1, 1, 1);

    QGridLayout* buttonsLayout = new QGridLayout;
    buttonsLayout->addWidget(savePB, 0, 0, 1, 1);
    buttonsLayout->addWidget(cancelPB, 0, 1, 1, 1);
    buttonsLayout->addWidget(defaultPB, 1, 1, 1, 1);

    systemScrollArea->setWidget(systemScrollWidget);
    systemScrollArea->setWidgetResizable(true);
    styleScrollArea->setWidget(styleScrollWidget);
    styleScrollArea->setWidgetResizable(true);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(systemScrollArea);
    mainLayout->addWidget(styleScrollArea);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    // connections
    connect(savePB, SIGNAL(pressed()), this, SLOT(saveSettings()));
    connect(cancelPB, SIGNAL(pressed()), this, SLOT(resetSettings()));
    connect(defaultPB, SIGNAL(pressed()), this, SLOT(resetSettingsToDefault()));
}

void OptionsWidget::_reset()
{
    initSettings = DefaultSettings;
    updateWidgets(initSettings);
    emit settingsChanged();
}

/*
    Updates widgets according to passed settings.
*/
void OptionsWidget::updateWidgets(const Settings& settings)
{
    if(!settings.cpuStatPath.isEmpty())
    {
        cpuStatPathLE->setText(settings.cpuStatPath);
    }
    if(!settings.memStatPath.isEmpty())
    {
        memStatPathLE->setText(settings.memStatPath);
    }
    if(!settings.netStatPath.isEmpty())
    {
        netStatPathLE->setText(settings.netStatPath);
    }

    dataPrecisionSB->setValue(settings.dataPrecision);
    gridOnOffCB->setChecked(settings.gridOn);
    languageCB->setCurrentIndex((int)settings.language);

    defaultChartsModeCB->setCurrentIndex((int)settings.defaultChartsMode);
}

// returns true if system settings has been changed
bool OptionsWidget::areSystemSettingsChanged() const
{
    return (Languages)languageCB->currentData().value<int>() != initSettings.language ||
            cpuStatPathLE->text() != initSettings.cpuStatPath ||
            memStatPathLE->text() != initSettings.memStatPath ||
            netStatPathLE->text() != initSettings.netStatPath;
}

// checks correctness of new stat paths
// returns true if everything is correct
bool OptionsWidget::checkStatPaths()
{
    try
    {
        CPUInfo cpuinfo(cpuStatPathLE->text().toStdString());
        MemInfo meminfo(memStatPathLE->text().toStdString());
        NetInfo netinfo(netStatPathLE->text().toStdString());
    }
    catch(...)
    {
        return false;
    }
    return true;
}

// if "Reset" pressed
void OptionsWidget::resetSettingsToDefault()
{
    if(QMessageBox::question(this, tr("Really?"), tr("This action will reset ALL settings to default."
                                                     "Are you really sure?")) == QMessageBox::Yes)
    {
        _reset();
    }
}

// if "Cancel" pressed
void OptionsWidget::resetSettings()
{ 
    updateWidgets(initSettings);
}

void OptionsWidget::saveSettings()
{
    if(!checkStatPaths())
    {
        QMessageBox::warning(this, tr("Error"), tr("Some new stat file path is incorrect. Settings can not be applied."));
        return;
    }
    Settings newSettings;
    newSettings.cpuStatPath = cpuStatPathLE->text();
    newSettings.memStatPath = memStatPathLE->text();
    newSettings.netStatPath = netStatPathLE->text();
    newSettings.dataPrecision = dataPrecisionSB->value();
    newSettings.gridOn = gridOnOffCB->isChecked();
    newSettings.defaultChartsMode = (Modes)(defaultChartsModeCB->currentData().value<int>());
    newSettings.language = (Languages)(languageCB->currentData().value<int>());

    bool systemChanged = areSystemSettingsChanged();
    if(systemChanged)
    {
        QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Warning"), tr("Some system settings has been changed. "
                                                      "To make them work you "
                                                      "should restart app. Restart?"));
        if(answer == QMessageBox::StandardButton::No) return;
    }
    initSettings = newSettings;
    emit settingsChanged();
    if(systemChanged)
    {
        emit systemSettingsChanged();
    }
}
