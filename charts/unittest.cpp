#include "unittest.hpp"

class SinCosUpdater
{
public:
    SinCosUpdater()
        : count{0} {}
    QVector<double> operator()()
    {
        QVector<double> values;

        values.push_back(sin((double)count++ / 100));
        values.push_back(cos((double)(3.14 + (double)count++ / 100)));

        return values;
    }
private:
    int count;
};

void RealtimeChartTest::Setting_Test()
{
    RealtimeChart rtc(1);
    // checking force max and min not growing
    rtc.setForceMaxEnabled(true);
    rtc.setForceMinEnabled(true);
    rtc.setForceMin(0);
    rtc.setForceMax(100);
    rtc.show();     // to call resize
    QRect chartsRect = rtc.getChartRect();

    QCOMPARE(rtc.getForceMax(), 100);
    QCOMPARE(rtc.getForceMin(), 0);

    // FORCE MAX AND MIN - RIGHT TO LEFT TESTS

    // ok - more than max + force
    QPoint point = rtc.getPoint(0, 101);
    QVERIFY(point.y() == chartsRect.top());

    // ok - less then min + force
    point = rtc.getPoint(1, -5.5);
    QVERIFY(point.y() == chartsRect.bottom());
    QVERIFY(point.x() == (int)(chartsRect.right() -  rtc.getSectionWidth()));   // because point is int type

    // ok - some inner points
    point = rtc.getPoint(50, 20);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * 20));
    QVERIFY(point.x() == (int)(chartsRect.right() -  rtc.getSectionWidth() * 50));   // because point is int type

    point = rtc.getPoint(77, 33);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * 33));
    QVERIFY(point.x() == (int)(chartsRect.right() -  rtc.getSectionWidth() * 77));   // because point is int type

    // FORCE MAX AND MIN - LEFT TO RIGHT TESTS

    rtc.setLeftToRight(true);

    // ok - more than max + force
    point = rtc.getPoint(0, 110);
    QVERIFY(point.y() == chartsRect.top());

    // ok - less then min + force
    point = rtc.getPoint(1, -1000);
    QVERIFY(point.y() == chartsRect.bottom());
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth()));   // because point is int type

    // ok - some inner points
    point = rtc.getPoint(88, 50);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * 50));
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth() * 88));   // because point is int type

    point = rtc.getPoint(2, 4);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * 4));
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth() * 2));   // because point is int type

    // MORE ONE FORCE MAX AND MIN

    rtc.setForceMax(10);
    rtc.setForceMin(-10);

    // ok - getters
    QCOMPARE(rtc.getForceMax(), 10);
    QCOMPARE(rtc.getForceMin(), -10);

    // ok - more than max + force
    point = rtc.getPoint(0, 110);
    QVERIFY(point.y() == chartsRect.top());

    // ok - less then min + force
    point = rtc.getPoint(1, -1000);
    QVERIFY(point.y() == chartsRect.bottom());
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth()));   // because point is int type

    // ok - some inner points
    point = rtc.getPoint(41, -8);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * (-8 - rtc.getForceMin())));
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth() * 41));   // because point is int type

    point = rtc.getPoint(2, 4);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * (4 - rtc.getForceMin())));
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth() * 2));   // because point is int type

    point = rtc.getPoint(31, 0);
    QVERIFY(point.y() == (int)(chartsRect.bottom() - rtc.getPointScale() * (0 - rtc.getForceMin())));
    QVERIFY(point.x() == (int)(chartsRect.left() +  rtc.getSectionWidth() * 31));   // because point is int type

    // DISABLING FORCE MAX
    RealtimeChart rtc2(2);
    // checking force max and min not growing
    rtc2.setForceMaxEnabled(false);
    rtc2.setForceMinEnabled(false);
    rtc2.show();     // to call resize
    SinCosUpdater scu;
    rtc2.setUpdaterFunction(scu);
    rtc2.setUpdaterTimer(10);
    chartsRect = rtc2.getChartRect();
    QTest::qWait(1000);

    // ok - some sinusoid drawn
    QVERIFY(rtc2.getForceMax() >= 0.9);
    QVERIFY(rtc2.getForceMin() <= -0.9);

    // MORE THAN ONE CHART
    RealtimeChart rtc3(6);
    QVERIFY(rtc3.getChartsCount() == 6);

    // MARGINS
    RealtimeChart rtc4(3);
    rtc4.setXMargins(QSize(40,50));
    rtc4.setYMargins(QSize(10,20));
    rtc4.show();
    QTest::qWait(10);

    // ok - checking charts rect coordinates follows margins
    QVERIFY(rtc4.mapToParent(rtc4.getChartRect().topLeft()) == QPoint(40,10));
    QVERIFY(rtc4.mapToGlobal(rtc4.rect().bottomRight())
            - rtc4.mapToGlobal(rtc4.getChartRect().bottomRight())
            == QPoint(50,20));

    // ok - ckecking stop/start correctness
    int timerValue = rtc.getUpdateTime();
    rtc4.stop();
    rtc4.start();
    QCOMPARE(timerValue, rtc4.getUpdateTime());

}
