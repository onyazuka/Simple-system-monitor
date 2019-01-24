#include "piechart.hpp"

PieChart::PieChart(QWidget *parent)
    : QWidget(parent), valuesSum{0}
{}

void PieChart::addValue(const QString &key, double value, const QString& suffix, QBrush brush, bool supressUpdate)
{
    values.push_back(std::move(Value(key, value)));
    palette.push_back(brush);
    suffixes.push_back(suffix);
    if(!supressUpdate)
    {
        updateRitual();
    }
}

// may be slow, but it is not designed to use it often
void PieChart::removeValue(const QString &key, bool supressUpdate)
{
    for(int i = 0; i < values.size(); ++i)
    {
        if(values[i].first == key)
        {
            values.removeAt(i);
            palette.removeAt(i);
            suffixes.removeAt(i);
            break;
        }
    }
    if(!supressUpdate)
    {
        updateRitual();
    }
}


void PieChart::clear(bool supressUpdate)
{
    values.clear();
    palette.clear();
    suffixes.clear();
    if(!supressUpdate)
    {
        updateRitual();
    }
}

void PieChart::setPen(const QPen &newPen)
{
    pen = newPen;
    emit iChanged();
    update();
}

void PieChart::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);

    // pie accepts 1/16 of degree as parameter
    const int FullAngle = 360 * 16;
    float angleOffset = 0;
    for(int i = 0; i < values.size(); ++i)
    {
        painter.setBrush(palette[i]);
        double curAngle = FullAngle * (values[i].second / valuesSum);
        painter.drawPie(QRect(margin, margin, width() - 2 * margin, height() - 2 * margin), angleOffset, curAngle);
        angleOffset += curAngle;
    }
}

void PieChart::updateRitual()
{
    emit iChanged();
    updateValuesSum();
    update();
}

/*
    Should be called each time when values added/removed
*/
void PieChart::updateValuesSum()
{
    valuesSum = 0;
    for(auto value: values)
    {
        valuesSum += value.second;
    }
}

