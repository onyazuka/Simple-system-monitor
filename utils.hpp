#pragma once
#include <QtCore>
#include "charts/chartsnamespace.hpp"
#include "applicationnamespace.hpp"

/*
    Wrapped in class for using Q_OBJECT and tr function.
*/
class Translator : public QObject
{
    Q_OBJECT
public:
    static QString fitBytes(double value, int precision);
    static QString languageToString(Languages language);
    static QString chartModeToString(Modes mode);
};
