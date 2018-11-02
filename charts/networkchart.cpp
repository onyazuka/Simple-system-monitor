#include "networkchart.hpp"

NetworkChart::NetworkChart(int chartsCount, QWidget *parent)
    : RealtimeChart(chartsCount, parent), mode{Modes::Invalid}, enableGrid{true}, dataPrecision{2}
{
    setForceMaxEnabled(false);
    setForceMinEnabled(false);
    setLeftToRight(false);
    setYMargins(QSize(0,30));
    setMode(Modes::Minute);
    setGridPen(QPen(QColor(180, 180, 180)));

    // configuring context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint&)));
}

/*
    Not parent's method!
    Also used for setting x margins!
*/
void NetworkChart::setLeftToRight(bool f)
{
    RealtimeChart::setLeftToRight(f);
    getLeftToRight() ? setXMargins(QSize(70, 30)) : setXMargins(QSize(30, 70));
}

/*
    WARNING! On time mode Minute user should update values each 100ms!
    On time mode Hour - each 5 seconds!
    User can get those values by calling getUpdateHint();
*/
void NetworkChart::setMode(Modes newMode)
{
    if(newMode == mode) return;
    mode = newMode;
    if(mode == Modes::Minute)
    {
        setMaxValues(120);  // by default - user can change this value
    }
    else if(mode == Modes::TenMinutes)
    {
        setMaxValues(600);
    }
    else if(mode == Modes::Hour)
    {
        setMaxValues(1200);
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

/*
    Makes from provided value (in bytes) kbytes, mbytes... and makes suffix (kb, mb etc)
    Returns resulting string.
*/
QString NetworkChart::fitBytes(double value)
{
    QString res;
    QString suffix;
    if(value < Sizes::KB)
    {
        suffix = tr("B");
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
    res = QString::number(value, 'f', dataPrecision) + suffix;
    return res;
}

/*
    Modes:
       minute - drawing labels each 10 seconds
       hour - drawing info labels each 5 minutes
*/
void NetworkChart::paintEvent(QPaintEvent* event)
{
    RealtimeChart::paintEvent(event);
    // initting
    QPainter painter(this);
    //drawing percents text
    QRect chartRect = getChartRect();
    int steps = 5;
    float yStep = (float)chartRect.height() / steps;
    int leftTextMargin = 10;
    int topTextMargin = 5;
    QFontMetrics fm(font());

    // drawing bytes info text and grid
    float traffic = 0;
    for(float y = chartRect.bottom(); y >= chartRect.top(); y -= yStep)
    {
        QString label = fitBytes(traffic);
        float xCoord = getLeftToRight() ?
                    chartRect.left() - leftTextMargin - fm.width(label):
                    chartRect.right() + leftTextMargin;
        float yCoord = y - fm.height() / 2;
        painter.save();
        painter.setPen(textPen);
        // well, fm.width() sometimes returns incorrect result, so I have added 5. May be wrong.
        painter.drawText(QRect(xCoord, yCoord,
                               fm.width(label) + 5, fm.height()),
                          label);
        if(enableGrid && y != chartRect.bottom() && y != chartRect.top())
        {
            painter.setPen(gridPen);
            painter.drawLine(chartRect.left(), y, chartRect.right(), y);
        }
        painter.restore();
        traffic += (float)getForceMax() / steps;
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

    steps = mode == Modes::TenMinutes ? 5 : 6;
    int timeUnits = 0;
    int timeUnitsStep = mode == Modes::TenMinutes ?
                10 / steps :
                60 / steps;;
    float xStep = (float)(endx - x) / steps;

    while(cond)
    {
        QString label;
        // if 0 seconds not drawing suffix
        if(timeUnits == 0)
        {
            label = QString("");
        }
        else
        {
            if(mode == Modes::Minute) label = tr("%1sec").arg(timeUnits);
            else if(mode == Modes::Hour || mode == Modes::TenMinutes) label = tr("%1min").arg(timeUnits);
        }
        painter.save();
        painter.setPen(textPen);
        painter.drawText(QRect(x - fm.width(label) / 2, yCoord,
                         fm.width(label), fm.height()),
                         label);

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
    Returns STRONGLY RECOMMENDED time interval for values updating(in ms)
*/
int NetworkChart::getUpdateHint()
{
    if(mode == Modes::Minute)
    {
        return 500;
    }
    else if (mode == Modes::TenMinutes)
    {
        return 1000;
    }
    else if(mode == Modes::Hour)
    {
        return 5000;
    }
    else        //
    {
        qWarning() << "Unknown mode";
        return -1;
    }
}

void NetworkChart::showContextMenu(const QPoint& point)
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
