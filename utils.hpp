#pragma once
#include <QtCore>
#include <unordered_map>
#include "charts/chartsnamespace.hpp"
#include "applicationnamespace.hpp"

/*
    Wrapped in class for using Q_OBJECT and tr function.
*/
class Translator : public QObject
{
    Q_OBJECT
public:
    typedef QPair<double, QString> SizedValue;
    static QString fitBytes(double value, int precision);
    static SizedValue fitBytesToNumber(double value, int precision);
    static QString languageToString(Languages language);
    static QString chartModeToString(Modes mode);
};

template<typename Key, typename Val>
QHash<Key, Val> unorderedMapToQHash(const std::unordered_map<Key, Val>& umap)
{
    QHash<Key, Val> res;
    for(auto pair: umap)
    {
        res.insert(pair.first, pair.second);
    }
    return std::move(res);
}
