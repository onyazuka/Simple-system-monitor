#pragma once
#include <QtWidgets>

/*
    Base abstract class for emulateable widgets
    (that supports emulation - working by start/stop).
    SHOULD update widget once in updateMsecs;
    SHOULD do its updates in updater function.
    CAN and if needed SHOULD use updater function passed in setUpdaterFunction to update its values.
    Supports starting/stoppings emulations.
*/
class EmulateableWidget : public QWidget
{
    Q_OBJECT
public:
    EmulateableWidget(QWidget* parent = nullptr);
    virtual ~EmulateableWidget() {}
    virtual void setUpdateInterval(int ms);
    template<typename F>
    void setUpdaterFunction(F f) {}
    virtual int getUpdateTime() const {return updateMsecs;}
    inline bool isRunning() const {return updateTimer.isActive();}

private:
    int updateMsecs;    // should update once per this value
    QTimer updateTimer;

public slots:
    virtual void start();
    virtual void stop();

protected slots:
    virtual void updater() {}
};
