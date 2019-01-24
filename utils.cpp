#include "utils.hpp"

/*
    Makes from provided value (in bytes) kbytes, mbytes... and makes suffix (kb, mb etc)
    Returns resulting string.
*/
QString Translator::fitBytes(double value, int precision)
{
    SizedValue sv = fitBytesToNumber(value, precision);
    QString res = QString::number(sv.first) + " " + sv.second;
    return res;
}

Translator::SizedValue Translator::fitBytesToNumber(double value, int precision)
{
    QString res;
    QString suffix;
    if(value < Sizes::KB)
    {
        suffix = tr("b");
    }
    else if(value < Sizes::MB)
    {
        value /= Sizes::KB;
        suffix = tr("KB");
    }
    else if(value < Sizes::GB)
    {
        value /= Sizes::MB;
        suffix = tr("MB");
    }
    else
    {
        value /= Sizes::GB;
        suffix = tr("GB");
    }
    return SizedValue(QString::number(value, 'd', precision).toDouble(), suffix);
}



/*
    Here we should register languages enum string representations.
*/
QString Translator::languageToString(Languages language)
{
    switch(language)
    {
    case Languages::English:
        return tr("English");
    case Languages::Russian:
        return tr("Russian");
    default:
        return QString();
    }
    return QString();
}

QString Translator::chartModeToString(Modes mode)
{
    switch(mode)
    {
    case Modes::Minute:
        return tr("Minute");
    case Modes::TenMinutes:
        return tr("Ten minutes");
    case Modes::Hour:
        return tr("Hour");
    default:
        return QString();
    }
}
