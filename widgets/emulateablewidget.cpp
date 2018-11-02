#include "emulateablewidget.hpp"

EmulateableWidget::EmulateableWidget(QWidget* parent)
    : QWidget{parent}
{
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updater()));
}

void EmulateableWidget::setUpdateInterval(int ms)
{
    updateMsecs = ms;
    updateTimer.stop();
    updateTimer.start(updateMsecs);
}

/*
    Starts updating activities
*/
void EmulateableWidget::start()
{
    updater();
    updateTimer.start(updateMsecs);
}

/*
    Stops any activity
*/
void EmulateableWidget::stop()
{
    updateTimer.stop();
}
