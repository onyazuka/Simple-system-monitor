#pragma once
#include "piechart.hpp"

class LabeledPiechart : public QWidget
{
    Q_OBJECT
public:
    // direction of LABELS relative to pieChart
    enum class Direction
    {
        TopLeft,
        TopCenter,
        TopRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
        Left,
        Right,
    };
    typedef QPair<QLabel*, QLabel*> ColorLabelPair;
    typedef QVector<ColorLabelPair> Labels;
    LabeledPiechart(Direction _direction , QWidget* parent=nullptr);
    PieChart* getPieChart() { return pieChart; }
    Direction getDirection() const { return direction; }
    size_t getLabelsCount() const { return labels.size(); }

public slots:
    void updateMe();

private:
    void createLabels();
    void createLayout();
    void makeConnections();
    void clearLayout(QLayout* layout);
    void updateLabelsSublayout();

    QVBoxLayout* labelsLayout;
    PieChart* pieChart;
    Direction direction;
    Labels labels;
};
