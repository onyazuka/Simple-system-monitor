#pragma once
#include "realtimechart.hpp"

/*
    Realtime chart for representing network processes
    x = time, y = traffic
    Has text info on coordinate system.
    Only difference from percent chart is drawing and some settings, hai
*/
class NetworkChart : public RealtimeChart
{
    Q_OBJECT
public:
    NetworkChart(int chartsCount, QWidget* parent = nullptr);

    // getters/setters
    void setLeftToRight(bool f);
    inline Modes getMode() const {return mode;}
    inline bool getEnableGrid() const {return enableGrid;}
    inline void setEnableGrid(bool f) {enableGrid = f; update();}
    inline QPen getGridPen() const {return gridPen;}
    inline void setGridPen(const QPen& pen) {gridPen = pen;}
    inline QPen getTextPen() const {return textPen;}
    inline void setTextPen(const QPen& pen) {textPen = pen;}
    inline int getDataPrecision() const {return dataPrecision;}
    inline void setDataPrecision(int dp) {dataPrecision = dp;}

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    int getUpdateHint();
    QString fitBytes(double val);

    Modes mode;
    bool enableGrid;
    QPen gridPen;
    QPen textPen;
    int dataPrecision;

public slots:
    void setMode(Modes newMode);

private slots:
    void showContextMenu(const QPoint& point);
};
