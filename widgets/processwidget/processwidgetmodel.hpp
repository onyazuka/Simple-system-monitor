#pragma once
#include <QtWidgets>
#include <mutex>
#include "core/procinfo.hpp"
#include "utils.hpp"

/*
    Uses QHash, but should I use it?
*/
class ProcessWidgetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns {PID, PPID, Name, CpuUsage, MemUsage, MemUsageBytes, HddRead, HddWrite, HddReadNow, HddWriteNow, COLUMN_COUNT};
    enum Roles {UserDataRole = Qt::UserRole + 1};
    ProcessWidgetModel(const QHash<int, ProcessActivities>& pActivities, QObject* parent=nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    const QHash<int, ProcessActivities> getProcessActivities() const { return processActivities; }
    void updateData(int key, const ProcessActivities& activities);
    void clearAndSetActivitiesMap(QHash<int, ProcessActivities>&& activities);
    int getRowByPID(int pid);

    inline int getDataPrecision() const { return dataPrecision; }
    inline void setDataPrecision(int _dataPrecision) { dataPrecision = _dataPrecision; }

private:
    int dataPrecision;
    QHash<int, ProcessActivities> processActivities;

    const QVector<QString> ColumnNames{tr("PID"), tr("Parent"), tr("Name"), tr("CPU Usage"),
                    tr("Memory Usage"), tr("Total Memory Usage"), tr("Total HDD Read"), tr("Total Hdd Write"),
                    tr("HDD Read"), tr("HDD Write")};
};

class ProxyInvalidModelException{};

class ProcessWidgetProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProcessWidgetProxyModel(QObject* parent = nullptr);

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};

