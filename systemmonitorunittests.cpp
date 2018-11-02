#include <QtTest>
#include "systemmonitor.hpp"

void SystemMonitorTest::Settings_Test()
{
    SystemMonitor sm;
    sm.show();
    Settings defaultSettings = DefaultSettings;

    // ok: init settings comparings
    QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
    QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);

    // ok: doing some changes in options and comparing once again
    sm.optionsWidget->gridOnOffCB->setChecked(false);
    sm.optionsWidget->dataPrecisionSB->setValue(0);
    sm.optionsWidget->defaultChartsModeCB->setCurrentIndex((int)Modes::TenMinutes);
    sm.optionsWidget->savePB->click();

    QCOMPARE(sm.appSettings.gridOn, false);
    QCOMPARE(sm.appSettings.dataPrecision, 0);
    QCOMPARE(sm.appSettings.defaultChartsMode, Modes::TenMinutes);
    QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
    QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);

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
    QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.memoryWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.networkWidget->getDataPrecision(), sm.appSettings.dataPrecision);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), sm.appSettings.gridOn);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), sm.appSettings.defaultChartsMode);
    QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), sm.appSettings.defaultChartsMode);

    // ok: doing reset
    sm.optionsWidget->_reset();
    QCOMPARE(sm.memoryWidget->getMemoryUsageChart().getEnableGrid(), defaultSettings.gridOn);
    QCOMPARE(sm.memoryWidget->getDataPrecision(), defaultSettings.dataPrecision);
    QCOMPARE(sm.networkWidget->getDataPrecision(), defaultSettings.dataPrecision);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getEnableGrid(), defaultSettings.gridOn);
    QCOMPARE(sm.networkWidget->getNetworkOutcomeChart().getEnableGrid(), defaultSettings.gridOn);
    QCOMPARE(sm.cpuWidget->getCpuChart(0).getMode(), defaultSettings.defaultChartsMode);
    QCOMPARE(sm.memoryWidget->getSwapUsageChart().getMode(), defaultSettings.defaultChartsMode);

    // ok: guarantee program is still working with incorrect path
    Settings setts = DefaultSettings;
    setts.cpuStatPath = "/proc/star";
    sm.applySettings(setts);    // ensure not throws
    // once again applying correct settings
    setts.cpuStatPath = "/proc/stat";
    sm.applySettings(setts);
}
