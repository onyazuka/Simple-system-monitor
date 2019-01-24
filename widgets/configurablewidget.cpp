#include "configurablewidget.hpp"

void ConfigurableWidget::setSettings(const Settings& _settings)
{
    setDataPrecision(_settings.dataPrecision);
    setChartGridEnabled(_settings.gridOn);
    setChartMode(_settings.defaultChartsMode);
}
