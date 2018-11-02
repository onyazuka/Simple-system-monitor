#pragma once
#include "realtimechart.hpp"

/*
    Realtime chart for representing long processes
    x = time, y = percents
    Has text info on coordinate system.
*/
class PercentTimeRealtimeChart : public RealtimeChart
{
    Q_OBJECT
public:
    PercentTimeRealtimeChart(int chartsCount, QWidget* parent = nullptr);

    // getters/setters
    inline Modes getMode() const {return mode;}
    inline bool getEnableGrid() const {return enableGrid;}
    inline void setEnableGrid(bool f) {enableGrid = f; update();}
    inline QPen getGridPen() const {return gridPen;}
    inline void setGridPen(const QPen& pen) {gridPen = pen;}
    inline QPen getTextPen() const {return textPen;}
    inline void setTextPen(const QPen& pen) {textPen = pen;}

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void paintEvent(QPaintEvent* event);

private:
    int getUpdateHint();

    Modes mode;
    bool enableGrid;
    QPen gridPen;
    QPen textPen;

public slots:
    void setMode(Modes newMode);

private slots:
    void showContextMenu(const QPoint& point);
};
