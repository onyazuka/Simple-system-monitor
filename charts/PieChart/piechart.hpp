#pragma once
#include <QtWidgets>
#include <algorithm>
#include <random>
#include <ctime>

class PieChartException
{
public:
    PieChartException(const std::string& _descr)
        : descr{_descr} {}
    const std::string& what() const{ return descr; }
private:
    std::string descr;
};


/*
    WARNING!
    vales and palette and suffixes arrays should always be synchronized!
*/
class PieChart : public QWidget
{
    Q_OBJECT
public:
    typedef QVector<QBrush> Palette;
    typedef QVector<QString> Suffixes;
    // don't using unordered map because of its unordered property
    typedef QPair<QString, double> Value;
    typedef QVector<QPair<QString, double>> Values;
    PieChart(QWidget* parent=nullptr);
    inline size_t size() const { return values.size(); }
    inline const Values& getValues() const { return values; }
    void setPen(const QPen& newPen);
    inline const QPen& getPen() const { return pen; }
    inline const Palette& getBrushesPallette() const { return palette; }
    inline const QBrush& getBrushAt(int i) const { return palette[i]; }
    inline const QString& getSuffixAt(int i) const { return suffixes[i]; }
    inline const Value& getValueAt(int i) const { return values[i]; }

public slots:
    void addValue(const QString& key, double value, const QString& suff, QBrush brush, bool supressUpdate=false);
    void removeValue(const QString& key, bool supressUpdate=false);
    void clear(bool supressUpdate=false);

protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void updateRitual();
    void updateValuesSum();

    // values and palette should be syncrohized
    Values values;
    Palette palette;
    Suffixes suffixes;

    QPen pen;
    double valuesSum;
    const int margin = 5;

signals:
    void iChanged();
};

