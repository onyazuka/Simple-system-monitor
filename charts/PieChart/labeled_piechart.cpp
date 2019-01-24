#include "labeled_piechart.hpp"

LabeledPiechart::LabeledPiechart(Direction _direction, QWidget *parent)
    : QWidget(parent), pieChart{new PieChart(this)}, direction{_direction}
{   
    createLabels();
    createLayout();
    makeConnections();
}

// should be called each time when some changes in the underlying pieChart has occured
void LabeledPiechart::updateMe()
{
    clearLayout(labelsLayout);
    labels.clear();
    createLabels();
    updateLabelsSublayout();
}

void LabeledPiechart::createLabels()
{
    const auto& values = pieChart->getValues();
    for(int i = 0; i < values.size(); ++i)
    {
        QLabel* newColorLabel = new QLabel(this);
        QLabel* newTextLabel = new QLabel(this);
        if(direction == Direction::Left || direction == Direction::Right)
        {
            newColorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            newTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        }
        else    //vertical direction
        {
            newColorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            newTextLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
        newColorLabel->setAutoFillBackground(true);
        QPalette newColorPalette = newColorLabel->palette();
        newColorPalette.setColor(newColorLabel->backgroundRole(), pieChart->getBrushAt(i).color());
        newColorLabel->setPalette(newColorPalette);
        newTextLabel->setText(values[i].first + ": " + QString::number(values[i].second) + " " + pieChart->getSuffixAt(i));
        labels.push_back(std::move(ColorLabelPair(newColorLabel, newTextLabel)));
    }
}

void LabeledPiechart::createLayout()
{
    labelsLayout = new QVBoxLayout();
    updateLabelsSublayout();
    switch(direction)
    {
    case Direction::TopLeft:
    case Direction::TopCenter:
    case Direction::TopRight:
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addLayout(labelsLayout, 1);
        mainLayout->addWidget(pieChart, 5);
        setLayout(mainLayout);
        break;
    }
    case Direction::BottomLeft:
    case Direction::BottomCenter:
    case Direction::BottomRight:
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(pieChart, 5);
        mainLayout->addLayout(labelsLayout, 1);
        setLayout(mainLayout);
        break;
    }
    case Direction::Left:
    {
        QHBoxLayout* mainLayout = new QHBoxLayout();
        mainLayout->addLayout(labelsLayout, 1);
        mainLayout->addWidget(pieChart, 5);
        setLayout(mainLayout);
        break;
    }
    case Direction::Right:
    {
        QHBoxLayout* mainLayout = new QHBoxLayout();
        mainLayout->addWidget(pieChart, 5);
        mainLayout->addLayout(labelsLayout, 1);
        setLayout(mainLayout);
        break;
    }
    default:
        throw PieChartException("Labeled pie chart: unknown direction");
        break;
    }
}

void LabeledPiechart::makeConnections()
{
    connect(pieChart, SIGNAL(iChanged()), this, SLOT(updateMe()));
}

void LabeledPiechart::clearLayout(QLayout* layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget())
        {
            delete item->widget();
        }
    }
}

void LabeledPiechart::updateLabelsSublayout()
{
    switch(direction)
    {
    case Direction::Left:
    case Direction::Right:
    case Direction::TopLeft:
    case Direction::BottomLeft:
    {
        for(int i = 0; i < labels.size(); ++i)
        {
            QHBoxLayout* rowLayout = new QHBoxLayout();
            rowLayout->addWidget(labels[i].first, 1, Qt::AlignLeft);
            rowLayout->addWidget(labels[i].second, 10, Qt::AlignLeft);
            labelsLayout->addLayout(rowLayout);
        }
        break;
    }
    case Direction::TopCenter:
    case Direction::BottomCenter:
    {
        for(int i = 0; i < labels.size(); ++i)
        {
            QHBoxLayout* rowLayout = new QHBoxLayout();
            rowLayout->addWidget(labels[i].first, 1, Qt::AlignRight);
            rowLayout->addWidget(labels[i].second, 1, Qt::AlignLeft);
            labelsLayout->addLayout(rowLayout);
        }
        break;
    }
    case Direction::TopRight:
    case Direction::BottomRight:
    {
        for(int i = 0; i < labels.size(); ++i)
        {
            QHBoxLayout* rowLayout = new QHBoxLayout();
            rowLayout->addWidget(labels[i].first, 10, Qt::AlignRight);
            rowLayout->addWidget(labels[i].second, 1, Qt::AlignLeft);
            labelsLayout->addLayout(rowLayout);
        }
        break;
    }
    default:
        throw PieChartException("Labeled pie chart: unknown direction");
        break;
    }
}
