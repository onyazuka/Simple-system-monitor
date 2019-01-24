#include "optionswidget.hpp"

/*
    Adding layout to scroll area memo:
        1. Create scroll area;
        2. Create container widget for layout;
        3. Create layout on this widget;
        4. Set this widget to scroll area;
        5. Add scroll area to main layout.
*/
OptionsWidget::OptionsWidget(const Settings& settings, QWidget *parent)
    : QWidget{parent}, initSettings{settings}
{
    createWidgets();
    updateWidgets(settings);
    createLayout();
    makeConnections();
}

void OptionsWidget::_reset()
{
    initSettings = DefaultSettings;
    updateWidgets(initSettings);
    emit settingsChanged();
}

void OptionsWidget::createWidgets()
{
    systemScrollArea = new QScrollArea(this);
    styleScrollArea = new QScrollArea(this);

    languageLabel = new QLabel(tr("Language: "));
    languageCB = new QComboBox;
    languageCB->addItem(Translator::languageToString(Languages::English), Languages::English);
    languageCB->addItem(Translator::languageToString(Languages::Russian), Languages::Russian);
    languageLabel->setBuddy(languageCB);
    languageLabel->setObjectName(propertyLabelsName);

    widgetsEnableLabel = new QLabel(tr("Enabled widgets: "));
    widgetsEnableLW = new QListWidget();
    updateWidgetsEnableList(initSettings);
    widgetsEnableLabel->setObjectName(propertyLabelsName);

    dataPrecisionLabel = new QLabel(tr("Data precision(digits): "));
    dataPrecisionSB = new QSpinBox;
    dataPrecisionLabel->setBuddy(dataPrecisionSB);
    dataPrecisionSB->setRange(0,3);
    dataPrecisionLabel->setObjectName(propertyLabelsName);

    gridOnOffLabel = new QLabel(tr("Grid on: "));
    gridOnOffCB = new QCheckBox;
    gridOnOffLabel->setBuddy(gridOnOffCB);
    gridOnOffLabel->setObjectName(propertyLabelsName);

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
}

void OptionsWidget::updateWidgetsEnableList(const Settings& _settings)
{
    widgetsEnableLW->clear();
    QListWidgetItem* cpuwi = new QListWidgetItem(tr("CPU"));
    _settings.enableCpu ? cpuwi->setCheckState(Qt::Checked) : cpuwi->setCheckState(Qt::Unchecked);
    widgetsEnableLW->addItem(cpuwi);
    QListWidgetItem* memwi = new QListWidgetItem(tr("Memory"));
    _settings.enableMem ? memwi->setCheckState(Qt::Checked) : memwi->setCheckState(Qt::Unchecked);
    widgetsEnableLW->addItem(memwi);
    QListWidgetItem* netwi = new QListWidgetItem(tr("Network"));
    _settings.enableNet ? netwi->setCheckState(Qt::Checked) : netwi->setCheckState(Qt::Unchecked);
    widgetsEnableLW->addItem(netwi);
    QListWidgetItem* hddwi = new QListWidgetItem(tr("Hdd"));
    _settings.enableHdd ? hddwi->setCheckState(Qt::Checked) : hddwi->setCheckState(Qt::Unchecked);
    widgetsEnableLW->addItem(hddwi);
    QListWidgetItem* procwi = new QListWidgetItem(tr("Processes"));
    _settings.enableProc ? procwi->setCheckState(Qt::Checked) : procwi->setCheckState(Qt::Unchecked);
    widgetsEnableLW->addItem(procwi);
}

void OptionsWidget::createLayout()
{
    QWidget* systemScrollWidget = new QWidget;
    QGridLayout* systemLayout = new QGridLayout(systemScrollWidget);
    QWidget* styleScrollWidget = new QWidget;
    QGridLayout* styleLayout = new QGridLayout(styleScrollWidget);

    int row = 0;
    systemLayout->addWidget(languageLabel, row, 0, 1, 1);
    systemLayout->addWidget(languageCB, row++, 1, 1, 1);
    systemLayout->addWidget(widgetsEnableLabel, row, 0, 1, 1);
    systemLayout->addWidget(widgetsEnableLW, row++, 1, 1, 1);

    styleLayout->addWidget(dataPrecisionLabel, row, 0, 1, 1);
    styleLayout->addWidget(dataPrecisionSB, row++, 1, 1, 1);
    styleLayout->addWidget(gridOnOffLabel, row, 0, 1, 1);
    styleLayout->addWidget(gridOnOffCB, row++, 1, 1, 1);
    styleLayout->addWidget(defaultChartsModeLabel, row, 0, 1, 1);
    styleLayout->addWidget(defaultChartsModeCB, row++, 1, 1, 1);

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
}

void OptionsWidget::makeConnections()
{
    connect(savePB, SIGNAL(pressed()), this, SLOT(saveSettings()));
    connect(cancelPB, SIGNAL(pressed()), this, SLOT(resetSettings()));
    connect(defaultPB, SIGNAL(pressed()), this, SLOT(resetSettingsToDefault()));
}

/*
    Updates widgets according to passed settings.
*/
void OptionsWidget::updateWidgets(const Settings& settings)
{
    dataPrecisionSB->setValue(settings.dataPrecision);
    gridOnOffCB->setChecked(settings.gridOn);
    languageCB->setCurrentIndex((int)settings.language);
    updateWidgetsEnableList(settings);
    defaultChartsModeCB->setCurrentIndex((int)settings.defaultChartsMode);
}

/*
    WARNING! Checking order should be the same, as the Adding order.
*/
bool OptionsWidget::areEnableWidgetsSettingsChanged() const
{
    if((bool)widgetsEnableLW->item(0)->checkState() != initSettings.enableCpu)
    {
        return true;
    }
    if((bool)widgetsEnableLW->item(1)->checkState() != initSettings.enableMem)
    {
        return true;
    }
    if((bool)widgetsEnableLW->item(2)->checkState() != initSettings.enableNet)
    {
        return true;
    }
    if((bool)widgetsEnableLW->item(3)->checkState() != initSettings.enableHdd)
    {
        return true;
    }
    if((bool)widgetsEnableLW->item(4)->checkState() != initSettings.enableProc)
    {
        return true;
    }
    return false;
}

// returns true if system settings has been changed
bool OptionsWidget::areSystemSettingsChanged() const
{   
    return (Languages)languageCB->currentData().value<int>() != initSettings.language ||
            areEnableWidgetsSettingsChanged();
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
    Settings newSettings;
    newSettings.dataPrecision = dataPrecisionSB->value();
    newSettings.gridOn = gridOnOffCB->isChecked();
    newSettings.defaultChartsMode = (Modes)(defaultChartsModeCB->currentData().value<int>());
    newSettings.language = (Languages)(languageCB->currentData().value<int>());
    newSettings.enableCpu = (bool)widgetsEnableLW->item(0)->checkState();
    newSettings.enableMem = (bool)widgetsEnableLW->item(1)->checkState();
    newSettings.enableNet = (bool)widgetsEnableLW->item(2)->checkState();
    newSettings.enableHdd = (bool)widgetsEnableLW->item(3)->checkState();
    newSettings.enableProc = (bool)widgetsEnableLW->item(4)->checkState();


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
