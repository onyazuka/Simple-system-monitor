#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "chartsnamespace.hpp"

class RealtimeChartException
{
public:
    RealtimeChartException(const std::string& _descr)
        : descr{_descr} {}
    const std::string& what() { return descr; }
private:
    std::string descr;
};

/*
    Builds chart in real time.
    draws chart for 'maxValues' values. User can call addValue method that updates graph.
    User can set force min and max values - if value is bigger than max or less than min, it will be counted as max/min;
        or user can not specify those values and chart will be calculate those values in runtime.
    User can also change chart and background pen/brushes.
    If user has not provided colors for charts, colors are taken from defaultPalette.
    User should provide chart builder with updater function and updater timer!
*/
class RealtimeChart : public QWidget
{
    Q_OBJECT
    typedef QList<double> Chart;        // as member of Charts
    typedef QVector<Chart> Charts;
    typedef QVector<QPen> ChartPens;
    typedef QVector<QBrush> ChartBrushes;
public:
    RealtimeChart(int chartsCount, QWidget* parent=nullptr);

    //reimplemented
    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

    // getters/setters
    inline int getChartsCount() const {return charts.size();}
    inline int getMaxValues() const {return maxValues;}
    void setMaxValues(int _maxValues);
    inline double getForceMin() const {return forceMin;}
    inline void setForceMin(double _forceMin) {forceMin = _forceMin;}
    inline double getForceMax() const {return forceMax;}
    inline void setForceMax(double _forceMax) {forceMax = _forceMax;}
    inline void setForceMinEnabled(bool f) {forceMinEnabled = f;}
    inline void setForceMaxEnabled(bool f) {forceMaxEnabled = f;}
    inline bool getLeftToRight() const {return leftToRight;}
    inline void setLeftToRight(bool _leftToRight) {leftToRight = _leftToRight;}
    inline QSize getXMargins() const {return xMargins;}
    inline void setXMargins(const QSize& x) {xMargins = x;}
    inline QSize getYMargins() const {return yMargins;}
    inline void setYMargins(const QSize& y) {yMargins = y;}

    inline QPen getChartPen(int index) const {return chartPens[index];}
    inline void setChartPen(int index, const QPen& pen) {chartPens[index] = pen;}
    inline QBrush getChartBrush(int index) const {return chartBrushes[index];}
    inline void setChartBrush(int index, const QBrush& brush) {chartBrushes[index] = brush;}
    inline QPen getBackgroundPen() const {return backgroundPen;}
    inline void setBackgroundPen(const QPen& _pen) {backgroundPen = _pen;}
    inline QBrush getBackgroundBrush() const {return backgroundBrush;}
    inline void setBackgroundBrush(const QBrush& _brush) {backgroundBrush = _brush;}
    inline const QVector<QColor>& getDefaultPalette() const {return defaultPalette;}
    inline int getUpdateTime() const {return updateTime;}
    void setDefaultPalette(const QVector<QColor>& newPalette);

    // for drawing purposes
    inline QRect getChartRect() const {return chartRect;}
    inline float getPointScale() const {return pointScale;}
    inline float getSectionWidth() const {return sectionWidth;}

    // other methods
    void clearPaths();
    template<typename F>
    void setUpdaterFunction(F f);
    void setUpdaterTimer(int updateTime);
    QPoint getPoint(int pointNumber, double value);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent* event);

private:
    void initializeDefaulPalette();
    void processUnforceMinMax(const QVector<double>& newValues);

    QLabel* chartLabel;
    QVector<QColor> defaultPalette;

    Charts charts;
    int maxValues;
    double forceMin;
    bool forceMinEnabled;
    double forceMax;
    bool forceMaxEnabled;
    bool leftToRight;
    QSize xMargins;
    QSize yMargins;
    ChartPens chartPens;
    ChartBrushes chartBrushes;
    QPen backgroundPen;
    QBrush backgroundBrush;
    int updateTime;

    //those 2 values used when force max or min are not set - for counting new max and min
        // when this max or min is gone
    int forceMaxEstablishedTurnsBack;
    int forceMinEstablishedTurnsBack;
    QRect chartRect;
    float pointScale;
    float sectionWidth;
    QTimer* updateTimer;

public slots:
    void start() { if(updateTimer) updateTimer->start(updateTime);}
    void stop() { if(updateTimer) updateTimer->stop();}
    void addValues(const QVector<double>& newValues);
    void addValues(const std::vector<double>& newValues);
};

/*
    Schedules callings of function f once per updateTime ms (that should be passed in setUpdaterTime()) to
    fetch some new data for chart.
    f MUST return QVector<double> or std::vector<double> of size no lesser than chartsCount
        and must be CALLABLE type!!!
*/
template<typename F>
void RealtimeChart::setUpdaterFunction(F f)
{
    /*we need to store f to call it in connect,
        so we are storing it in dynamic memory with std::shared_ptr to delete it later*/
    std::shared_ptr<F> updF {new F(f)};
    if(updateTimer == nullptr) updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this, updF](){addValues(std::move((*updF)()));});
}
