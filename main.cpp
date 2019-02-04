#include <gtest/gtest.h>
#include <QtTest>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "systemmonitor.hpp"
#include "charts/unittest.hpp"
#include "systemmonitorunittests.hpp"

const QString GlobalStylesheet = QString(
                                 "QProgressBar {text-align: center;}"
                                 "*{font-size: 12px; font-family: Loma, serif;}"
                                 "QLabel#%1 {font-size: 13px; font-weight: bold; font-family: Ebrima, sans-serif;}"
                                 "QLabel#%2 {font-size: 12px; font-weight: bold; font-family: Ebrima, sans-serif; }"
                                 "QLabel#%3 {font-size: 12px; font-family: Loma, serif; }"
                                 "QLabel#%4 {font-size: 12px; font-family: Euphemia, serif; }"
                                 "QTabBar::tab { width: 50px; }"
                                 "QTabBar::tab:selected { background-color: black; }"
                                 "QTabBar::tab:hover { background-color: #aaa; }"
                                 "QTableView::item:hover { background-color: #aaa; }"
                                 "QComboBox:hover { background-color: #aaa; }"
                                 "QListWidget::item:hover { background-color: #aaa; }"
                                  "QPushButton:hover { background-color: #aaa; }"
                                         ).arg(titleLabelsName, chartDescriptionName, infoLabelsName, propertyLabelsName);


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
        app.setWindowIcon(QIcon(":/icons/icon.png"));
        SystemMonitor sysmon;
        sysmon.show();
        exitCode = app.exec();
        if(exitCode != RestartExitCode) break;
    }
    return exitCode;
}
