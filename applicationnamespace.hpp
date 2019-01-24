#pragma once
#include <QString>
#include <QColor>

enum Languages{English, Russian};
enum Tabs{Total, CPU, Memory, Net, Hdd, Proc};
typedef int PID;

const int RestartExitCode = 1000;

// object names for styling
const QString chartDescriptionName = "Chart";
const QString titleLabelsName =     "Title";
const QString propertyLabelsName =  "Property";
const QString infoLabelsName =      "Info";

const QColor hddBusyColor  = QColor(50,58,131);
const QColor hddAvailColor = QColor(50,155,104);
