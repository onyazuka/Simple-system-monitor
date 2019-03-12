#include "percenttimerealtimechart.hpp"

PercentTimeRealtimeChart::PercentTimeRealtimeChart(int chartsCount, QWidget *parent)
    : RealtimeChart(chartsCount, parent), mode{Modes::Invalid}, enableGrid{true}
{
    setForceMaxEnabled(true);
    setForceMinEnabled(true);
    setForceMax(100);
    setForceMin(0);
    setLeftToRight(false);
    setXMargins(QSize(40,40));
    setYMargins(QSize(0,30));
    setMode(Modes::Minute);
    setGridPen(QPen(QColor(180, 180, 180)));

    // configuring context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint&)));
}

/*
    WARNING! On time mode Minute user should update values each 100ms!
    On time mode Hour - each 5 seconds!
    User can get those values by calling getUpdateHint();
*/
void PercentTimeRealtimeChart::setMode(Modes newMode)
{
    if(newMode == mode) return;
    mode = newMode;
    if(mode == Modes::Minute)
    {
        setMaxValues(120);  // by default - user can change this value
    }
    else if(mode == Modes::TenMinutes)
    {
        setMaxValues(200);
    }
    else if(mode == Modes::Hour)
    {
        setMaxValues(200);
    }
    else
    {
        qWarning("Sonna moodo shiranai, baka");
        return;
    }
    clearPaths();
    update();
    setUpdaterTimer(getUpdateHint());
}

void PercentTimeRealtimeChart::resizeEvent(QResizeEvent* event)
{
    RealtimeChart::resizeEvent(event);
    update();
}

/*
    Modes:
       minute - drawing labels each 10 seconds
       hour - drawing info labels each 5 minutes
*/
void PercentTimeRealtimeChart::paintEvent(QPaintEvent* event)
{
    RealtimeChart::paintEvent(event);
    // initting
    QPainter painter(this);
    //drawing percents text
    QRect chartRect = getChartRect();
    float yStep = (float)chartRect.height() / 5;
    int leftTextMargin = 10;
    int topTextMargin = 5;
    QFontMetrics fm(font());

    // drawing percents text and grid
    int percents = 0;
    for(float y = chartRect.bottom(); y >= chartRect.top(); y -= yStep)
    {
        if(percents == 100)
        {
            continue;
        }
        QString label = QString::number(percents) + "%";
        float xCoord = getLeftToRight() ?
                    chartRect.left() - leftTextMargin - fm.width(label):
                    chartRect.right() + leftTextMargin;
        float yCoord = y - fm.height() / 2;
        painter.save();
        painter.setPen(textPen);
        // not drawing labels if it is too little space
        if(fm.height() * 0.7 <= yStep)
        {
            painter.drawText(QRect(xCoord, yCoord,
                                   fm.width(label), fm.height()),
                              label);
        }
        // drawing grid, but not drawing it in bottom and top
        if(enableGrid && y != chartRect.bottom() && y != chartRect.top())
        {
            painter.setPen(gridPen);
            painter.drawLine(chartRect.left(), y, chartRect.right(), y);
        }
        painter.restore();
        percents += 20;
    }

    // drawing time text and grid
    float x = getLeftToRight() ?
                chartRect.left() :
                chartRect.right();
    float endx = getLeftToRight() ?
                chartRect.right() :
                chartRect.left();
    bool cond = getLeftToRight() ?
                ceil(x) <= endx :
                ceil(x) >= endx;
    int yCoord = chartRect.bottom() + topTextMargin;

    int steps = mode == Modes::TenMinutes ? 5 : 6;
    int timeUnits = 0;
    int timeUnitsStep = mode == Modes::TenMinutes ?
                10 / steps :
                60 / steps;;
    float xStep = (float)(endx - x) / steps;

    while(cond)
    {
        QString label;
        QString suffixlessLabel;
        // if 0 seconds not drawing suffix
        if(timeUnits == 0)
        {
            label = QString("");
        }
        else
        {
            if(mode == Modes::Minute)
            {
                label = tr("%1sec").arg(timeUnits);
                suffixlessLabel = tr("%1").arg(timeUnits);
            }
            else if(mode == Modes::Hour || mode == Modes::TenMinutes)
            {
                label = tr("%1min").arg(timeUnits);
                suffixlessLabel = tr("%1").arg(timeUnits);
            }
        }
        painter.save();
        painter.setPen(textPen);
        if(fm.width(label) <= fabs(xStep))   // abs because step can be negative(if moving from right to left)
        {
            painter.drawText(QRect(x - fm.width(label) / 2, yCoord,
                             fm.width(label), fm.height()),
                             label);
        }
        else if(fm.width(suffixlessLabel) <= fabs(xStep))
        {
            painter.drawText(QRect(x - fm.width(suffixlessLabel) / 2, yCoord,
                             fm.width(suffixlessLabel), fm.height()),
                             suffixlessLabel);
        }
        // else drawing nothing

        //grid
        if(enableGrid && x != chartRect.left() && x != chartRect.right())
        {
            painter.setPen(gridPen);
            painter.drawLine(x, chartRect.top(),
                             x, chartRect.bottom());
        }
        painter.restore();
        // if from right to left step is already negative
        x += xStep;
        timeUnits += timeUnitsStep;
        cond = getLeftToRight() ?
               ceil(x) <= endx :
               ceil(x) >= endx;
    }
}

/*
    private method that returns time interval for updateTimer for values updating(in ms)
*/
int PercentTimeRealtimeChart::getUpdateHint()
{
    if(mode == Modes::Minute)
    {
        return 500;
    }
    else if (mode == Modes::TenMinutes)
    {
        return 5000;
    }
    else if(mode == Modes::Hour)
    {
        return 30000;
    }
    else
    {
        qWarning() << "unknown mode";
        return -1;
    }
}

void PercentTimeRealtimeChart::showContextMenu(const QPoint& point)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction setMinuteModeAction(tr("Show next minute"), this);
    connect(&setMinuteModeAction, &QAction::triggered,
            [this](){this->setMode(Modes::Minute);});
    contextMenu.addAction(&setMinuteModeAction);

    QAction setTenMinutesModeAction(tr("Show next ten minutes"), this);
    connect(&setTenMinutesModeAction, &QAction::triggered,
            [this](){this->setMode(Modes::TenMinutes);});
    contextMenu.addAction(&setTenMinutesModeAction);

    QAction setHourModeAction(tr("Show next hour"), this);
    connect(&setHourModeAction, &QAction::triggered,
            [this](){this->setMode(Modes::Hour);});
    contextMenu.addAction(&setHourModeAction);

    contextMenu.exec(mapToGlobal(point));
}
