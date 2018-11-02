#pragma once
#include <QtTest>
#include <QTest>
#include "realtimechart.hpp"

class RandomValuesAdder
{
public:
    RandomValuesAdder(int ch)
        : charts{ch} {}
    QVector<double> operator()()
    {
        QVector<double> values;
        for(int i = 0; i < charts; ++i)
        {
            values.push_back(rand() % 100);
        }
        return values;
    }
private:
    int charts;
};

class LinearValuesAdder
{
public:
    LinearValuesAdder(int ch, int start)
        : charts{ch}, counter{start} {}
    QVector<double> operator()()
    {
        QVector<double> values;
        for(int i = 0; i < charts; ++i)
        {
            values.push_back(counter++);
        }
        return values;
    }
private:
    int charts;
    int counter;
};

class RealtimeChartTest : public QObject
{
    Q_OBJECT
private slots:
    void Setting_Test();
};
