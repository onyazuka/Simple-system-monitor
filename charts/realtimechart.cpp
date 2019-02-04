#include "realtimechart.hpp"

RealtimeChart::RealtimeChart(int chartsCount, QWidget* parent)
    : QWidget{parent} , maxValues{600}, forceMin{0}, forceMinEnabled{false},
      forceMax{0.01}, forceMaxEnabled{false}, leftToRight{false},
      xMargins{QSize(0,0)}, yMargins{QSize(0,0)}, updateTime(500), drawUnderLine{false},
      forceMaxEstablishedTurnsBack{0}, forceMinEstablishedTurnsBack{0}, pointScale{0},
      sectionWidth{0}, updateTimer{nullptr}
{
    initializeDefaulPalette();

    //initializing vetors
    charts.resize(chartsCount);
    chartPens.resize(chartsCount);
    chartBrushes.resize(chartsCount);
    for(int i = 0; i < chartsCount; ++i)
    {
        charts[i] = Chart();
        chartPens[i] = QPen(defaultPalette[rand() % defaultPalette.size()]);
        chartBrushes[i] = QBrush(QColor(chartPens[i].color().red(), chartPens[i].color().green(), chartPens[i].color().blue(), 128));
    }
    chartLabel = new QLabel;
    update();
}

void RealtimeChart::setMaxValues(int _maxValues)
{
    maxValues = _maxValues;
    if(maxValues < 0)
    {
        throw RealtimeChartException("RealtimeChart::setMaxValues(_maxValues): _maxValues can not be less then 0");
    }
}

QSize RealtimeChart::minimumSizeHint() const
{
    return QSize(200,80);
}

QSize RealtimeChart::sizeHint() const
{
    return QSize(350,130);
}

/*
    Sets charts pen colors to random color from newPalette.
*/
void RealtimeChart::setDefaultPalette(const QVector<QColor>& newPalette)
{
    for(int i = 0; i < getChartsCount(); ++i)
    {
        chartPens[i] = QPen(newPalette[rand() % newPalette.size()]);
        chartBrushes[i] = QBrush(QColor(chartPens[i].color().red(), chartPens[i].color().green(), chartPens[i].color().blue(), 64));
    }
}

void RealtimeChart::clearPaths()
{
    for(int i = 0; i < getChartsCount(); ++i)
    {
        charts[i].clear();
    }
}

/*
    Usually should be called WITH setting updater function
*/
void RealtimeChart::setUpdaterTimer(int _updateTime)
{
    if(updateTimer != nullptr) updateTimer->stop();
    else updateTimer = new QTimer(this);
    updateTime = _updateTime;
    updateTimer->start(updateTime);
}

/*
    Gets point coordinates for drawing in chart.
    PointNumber is used for calculating x coordinate, value - for y coordinate.
    Should account leftToRight and force values.
    Made it public for testing purposes.
*/
QPoint RealtimeChart::getPoint(int pointNumber, double value)
{
    int pointX = leftToRight ? chartRect.left() : chartRect.right();
    pointX += leftToRight ? sectionWidth * pointNumber : -sectionWidth * pointNumber;
    // substracting forceMin needed where forceMin is not 0
    int pointY = chartRect.bottom() - (value - forceMin) * pointScale;
    if(forceMaxEnabled && (value > forceMax))
    {
        pointY = chartRect.top();
    }
    if(forceMinEnabled && (value < forceMin))
    {
        pointY = chartRect.bottom();
    }
    return QPoint(pointX, pointY);
}

/*
    Updates painter on resize.
    Also updates sizes of chartRect - it is better to do it here, not in paint event.
*/
void RealtimeChart::resizeEvent(QResizeEvent *event)
{
    int chartXBegin = xMargins.width();
    // height is just second parameter, stay cool
    int chartWidth = width() - xMargins.width() - xMargins.height();
    int chartYBegin = yMargins.width();
    int chartHeight = height() - yMargins.width() - yMargins.height();

    // not drawing if not needed
    // ALSO not updating chartRect that is important!
    if(chartWidth <= 10 || chartHeight <= 10)
    {
        return;
    }

    chartRect = QRect(chartXBegin, chartYBegin, chartWidth, chartHeight);
    sectionWidth = (float)chartWidth / maxValues;
    //using bottom - top, because we don't want rect's borders to be counted
    pointScale = (chartRect.bottom() - chartRect.top()) / (forceMax - forceMin);

    QWidget::resizeEvent(event);
    update();
}

/*
    TO OPTIMIZE: paintes paths of big size drawing is slow!
*/
void RealtimeChart::paintEvent(QPaintEvent* event)
{
    if(getChartsCount() == 0 ||
      (forceMax - forceMin) == 0 ||
       maxValues == 0)
    {
        throw RealtimeChartException("RealtimeChart::paintEvent(event): can't draw - check args");
    }

    //using bottom - top, because we don't want rect's borders to be counted
    sectionWidth = (float)chartRect.width() / maxValues;
    pointScale = (chartRect.bottom() - chartRect.top()) / (forceMax - forceMin);

    //initting
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // drawing background
    painter.save();
    painter.setPen(backgroundPen);
    painter.setBrush(backgroundBrush);
    painter.drawRect(chartRect);
    painter.restore();

    // no warning because not critical, but this check is useful
    // should be here, not earler, becuse in case of this error we want to update chartRect
    if(charts[0].size() == 0)
    {
        return;
    }

    // drawing chart
    for(int chart = 0; chart < getChartsCount(); ++chart)
    {
        if(charts[chart].empty())
        {
            continue;
        }
        QPainterPath path;
        painter.setPen(chartPens[chart]);
        path.moveTo(getPoint(0, charts[chart][0]));
        for(int point = 1; point < charts[chart].size(); ++point)
        {
            // one time point can not have 2 or more values
            path.lineTo(getPoint(point, charts[chart][point]));
        }
        painter.drawPath(path);
        if(drawUnderLine)
        {
            // closing path and filling it
            painter.setBrush(chartBrushes[chart]);
            path.lineTo(getPoint(charts[chart].size() - 1, 0));
            path.lineTo(getPoint(0, 0));
            path.lineTo(getPoint(0, charts[chart][0]));
            painter.fillPath(path, chartBrushes[chart]);
        }
    }
    QWidget::paintEvent(event);
}

void RealtimeChart::initializeDefaulPalette()
{
    defaultPalette
            << QColor("red")
            << QColor("brown")
            << QColor("purple")
            << QColor("orange")
            << QColor("green")
            << QColor("cyan")
            << QColor("blue")
            << QColor("magenta");
}

/*
    updating force min and/or max if unforce mode is on
    can select new min/max from newValues.
*/
void RealtimeChart::processUnforceMinMax(const QVector<double>& newValues)
{
    // selecting(maybe) new min/max
    double newValuesMax = *std::max_element(newValues.begin(), newValues.end());
    double newValuesMin = *std::min_element(newValues.begin(), newValues.end());

    // if !forceMax - floating max enabled
    if(!forceMaxEnabled && newValuesMin < forceMin)
    {
        forceMin = newValuesMin;
        forceMaxEstablishedTurnsBack = 0;
    }
    if(!forceMinEnabled && newValuesMax > forceMax)
    {
        forceMax = newValuesMax;
        forceMinEstablishedTurnsBack = 0;
    }
    // if old min/max has gone - finding new
    if(!forceMaxEnabled && forceMaxEstablishedTurnsBack >= maxValues)
    {
        forceMax = std::numeric_limits<double>::min();
        for(auto chart : charts)
        {
            double tempMax = forceMax;
            tempMax = *std::max_element(chart.begin(), chart.end());
            forceMax = forceMax > tempMax ? forceMax : tempMax;
        }
        forceMaxEstablishedTurnsBack = 0;
    }

    if(!forceMinEnabled && forceMinEstablishedTurnsBack >= maxValues)
    {
        forceMin = std::numeric_limits<double>::max();
        for(auto chart : charts)
        {
            double tempMin = forceMin;
            tempMin = *std::min_element(chart.begin(), chart.end());
            forceMin = forceMin < tempMin ? forceMin : tempMin;
        }
        forceMinEstablishedTurnsBack = 0;
    }
}

/*
    If values already have maxValues values, deletes value from back.
    Pushes values on front.
    newValues size should be EQUAL to charts size,
        or BIGGER(but than only first values would be taken).
*/
void RealtimeChart::addValues(const QVector<double>& newValues)
{
    if(newValues.size() < getChartsCount())
    {
        throw std::runtime_error("RealTimeChart::addValues(): newValues size should be EQUAL "
                                 "or bigger, than chartsCount!");
    }
    // I hope qlist::size() complexity is O(1)
    if(charts.front().size() == maxValues)
    {
        // popping from ALL charts
        for(int i = 0; i < getChartsCount(); ++i)
        {
            charts[i].pop_back();
        }
    }
    // pushing to ALL charts
    for(int i = 0; i < getChartsCount(); ++i)
    {
        charts[i].push_front(newValues[i]);
    }

    if(!forceMaxEnabled || !forceMinEnabled)
    {
        processUnforceMinMax(newValues);
    }
    ++forceMaxEstablishedTurnsBack;
    ++forceMinEstablishedTurnsBack;
    update();
}

void RealtimeChart::addValues(const std::vector<double>& newValues)
{
    QVector<double> qvector = QVector<double>::fromStdVector(newValues);
    addValues(qvector);
}


