#include <gtest/gtest.h>
#include <QtTest>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "systemmonitor.hpp"
#include "charts/unittest.hpp"
#include "applicationnamespace.hpp"

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication app(argc, argv);
    app.setOrganizationName("Onyazuka_Sofuto");
    app.setOrganizationDomain("");
    app.setApplicationName("Simple_system_monitor");
    QSettings::setDefaultFormat(QSettings::IniFormat);

#ifdef QT_DEBUG
    // core tests
    ::testing::InitGoogleTest(&argc, argv);
    int err =  RUN_ALL_TESTS();
    if(err)
    {
        return err;
    }

    // charts tests
    QTest::qExec(new RealtimeChartTest, argc, argv);
    QTest::qExec(new SystemMonitorTest, argc, argv);
#endif
    int exitCode = 0;
    QTranslator translator;
    while(true)
    {
        QSettings settings;

        if(settings.contains("Language") && (Languages)settings.value("Language").value<int>() ==
                Languages::Russian)
        {
            translator.load(":/ru_translation");
            app.installTranslator(&translator);
        }
        else
        {
            app.removeTranslator(&translator);
        }
        app.setStyleSheet(GlobalStylesheet);
        SystemMonitor sysmon;
        sysmon.show();
        exitCode = app.exec();
        if(exitCode != RestartExitCode) break;
    }
    return exitCode;
}
