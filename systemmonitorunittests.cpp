#include <QtTest>
#include "systemmonitorunittests.hpp"

void SystemMonitorTest::Settings_Test()
{
    SystemMonitor sm;
    Settings defaultSettings = DefaultSettings;

    // ok: init settings comparings

    if(sm.infoManager->isMemInfoInitialized())
    {
        QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);
    }
    if(sm.infoManager->isNetInfoInitialized())
    {
        QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    }
    if(sm.infoManager->isCpuInfoInitialized())
    {
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
    }

    // ok: doing some changes in options and comparing once again
    sm.optionsWidget->gridOnOffCB->setChecked(false);
    sm.optionsWidget->dataPrecisionSB->setValue(0);
    sm.optionsWidget->defaultChartsModeCB->setCurrentIndex((int)Modes::TenMinutes);
    sm.optionsWidget->savePB->click();

    QCOMPARE(sm.appSettings.gridOn, false);
    QCOMPARE(sm.appSettings.dataPrecision, 0);
    QCOMPARE(sm.appSettings.defaultChartsMode, Modes::TenMinutes);

    if(sm.infoManager->isMemInfoInitialized())
    {
        QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), sm.appSettings.gridOn);
        QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);
    }
    if(sm.infoManager->isCpuInfoInitialized())
    {
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
    }
    if(sm.infoManager->isNetInfoInitialized())
    {
        QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    }

    // ok: doing some changes but cancelling
    sm.optionsWidget->gridOnOffCB->setChecked(true);
    sm.optionsWidget->dataPrecisionSB->setValue(1);
    sm.optionsWidget->defaultChartsModeCB->setCurrentIndex((int)Modes::Hour);
    sm.optionsWidget->cancelPB->click();

    QCOMPARE(sm.optionsWidget->gridOnOffCB->isChecked(), false);
    QCOMPARE(sm.optionsWidget->dataPrecisionSB->value(), 0);
    QCOMPARE(sm.optionsWidget->defaultChartsModeCB->currentIndex(), (int)Modes::TenMinutes);
    QCOMPARE(sm.appSettings.gridOn, false);
    QCOMPARE(sm.appSettings.dataPrecision, 0);
    QCOMPARE(sm.appSettings.defaultChartsMode, Modes::TenMinutes);

    if(sm.infoManager->isMemInfoInitialized())
    {
        QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), sm.appSettings.gridOn);
        QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);
    }
    if(sm.infoManager->isCpuInfoInitialized())
    {
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
    }
    if(sm.infoManager->isNetInfoInitialized())
    {
        QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
        QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    }

    // ok: doing reset
    sm.optionsWidget->_reset();
    if(sm.infoManager->isMemInfoInitialized())
    {
        QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), defaultSettings.gridOn);
        QCOMPARE(sm.memoryWidget->getDataPrecision(), defaultSettings.dataPrecision);
        QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), defaultSettings.defaultChartsMode);
    }
    if(sm.infoManager->isCpuInfoInitialized())
    {
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), defaultSettings.gridOn);
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), defaultSettings.defaultChartsMode);
    }
    if(sm.infoManager->isNetInfoInitialized())
    {
        QCOMPARE(sm.networkWidget->getDataPrecision(), defaultSettings.dataPrecision);
        QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), defaultSettings.gridOn);
    }

    // and testing direct options applying, without optionsWidget
    Settings settings;
    settings.gridOn = true;
    settings.dataPrecision = 5;
    settings.defaultChartsMode = Modes::Hour;
    sm.applySettings(settings);

    if(sm.infoManager->isMemInfoInitialized())
    {
        QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), settings.gridOn);
        QCOMPARE(sm.memoryWidget->getDataPrecision(), settings.dataPrecision);
        QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), settings.defaultChartsMode);
    }
    if(sm.infoManager->isCpuInfoInitialized())
    {
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), settings.gridOn);
        QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), settings.defaultChartsMode);
    }
    if(sm.infoManager->isNetInfoInitialized())
    {
        QCOMPARE(sm.networkWidget->getDataPrecision(), settings.dataPrecision);
        QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), settings.gridOn);
    }
}

/*
    main purpose of this test is to check that program not crashes without a widget or two
*/
void SystemMonitorTest::WithoutSomeWidgets_Test()
{
    try
    {
        SystemMonitor sm;
        Settings savedSettings = sm.getAppSettings();
        Settings newSettings;
        newSettings.enableMem = true;
        newSettings.enableCpu = false;
        newSettings.enableNet = false;
        newSettings.dataPrecision = 2;
        newSettings.gridOn = true;
        newSettings.defaultChartsMode = Modes::TenMinutes;
        sm.appSettings = newSettings;
        sm.close();
        SystemMonitor sm2;
        sm2.show();
        sm2.appSettings = savedSettings;
        sm2.close();
    }
    catch(...)
    {
        qDebug() << "ME";
        QVERIFY(8==0);
    }
}
