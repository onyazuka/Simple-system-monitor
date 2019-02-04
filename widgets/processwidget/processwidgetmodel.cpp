#include "processwidgetmodel.hpp"

//------------------------------------------------MODEL--------------------------------------

ProcessWidgetModel::ProcessWidgetModel(const QHash<int, ProcessActivities> &pActivities, QObject *parent)
    : QAbstractTableModel{parent}, dataPrecision{2}, processActivities{pActivities} { }

int ProcessWidgetModel::rowCount(const QModelIndex&) const
{
    return processActivities.size();
}

int ProcessWidgetModel::columnCount(const QModelIndex&) const
{
    return COLUMN_COUNT;
}

/*
    Data can be for display and for getting real value(role == UserDataRole) for sorting.
*/
QVariant ProcessWidgetModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !(role == Qt::DisplayRole || role == UserDataRole))
    {
        return QVariant();
    }
    auto iter = processActivities.begin() + index.row();
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case PID:
            return QVariant(iter->pid);
        case PPID:
            return QVariant(QString::number(iter->ppid));
        case Name:
            return QVariant(QString::fromStdString(iter->name));
        case CpuUsage:
            return QVariant(QString::number((int)(iter->cpuUsage * 100)) + QString(" %"));
        case MemUsage:
            return QVariant(QString::number((int)(iter->memUsage * 100)) + QString(" %"));
        case MemUsageBytes:
            return QVariant(Translator::fitBytes(iter->memUsageBytes, dataPrecision));
        case HddRead:
            return QVariant(Translator::fitBytes(iter->ioRead, dataPrecision));
        case HddWrite:
            return QVariant(Translator::fitBytes(iter->ioWrite, dataPrecision));
        case HddReadNow:
            return QVariant(Translator::fitBytes(iter->ioReadNow, dataPrecision));
        case HddWriteNow:
            return QVariant(Translator::fitBytes(iter->ioWriteNow, dataPrecision));
        default:
            return QVariant();
        }
    }
    else if (role == UserDataRole)
    {
        switch(index.column())
        {
        case PID:
            return QVariant(iter->pid);
        case PPID:
            return QVariant(iter->ppid);
        case Name:
            return QVariant(QString::fromStdString(iter->name));
        case CpuUsage:
            return QVariant((double)iter->cpuUsage * 100);
        case MemUsage:
            return QVariant((double)iter->memUsage * 100);
        case MemUsageBytes:
            return QVariant(iter->memUsageBytes);
        case HddRead:
            return QVariant((double)iter->ioRead);
        case HddWrite:
            return QVariant((double)iter->ioWrite);
        case HddReadNow:
            return QVariant((double)iter->ioReadNow);
        case HddWriteNow:
            return QVariant((double)iter->ioWriteNow);
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant ProcessWidgetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section < 0 || section >= columnCount() || role != Qt::DisplayRole || orientation == Qt::Vertical)
    {
        return QVariant();
    }
    return ColumnNames[section];
}

void ProcessWidgetModel::updateData(int key, const ProcessActivities& activities)
{
    processActivities[key] = activities;
    int row = std::distance(processActivities.begin(), processActivities.find(key));
    emit dataChanged(index(row, 0), index(row, 0));
}

/*
    Clears old qhash and sets new in one step
*/
void ProcessWidgetModel::clearAndSetActivitiesMap(QHash<int, ProcessActivities>&& activities)
{
    beginRemoveRows(QModelIndex(), 0, processActivities.size() - 1);
    processActivities.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, activities.size() - 1);
    processActivities = std::move(activities);
    endInsertRows();
    emit dataChanged(index(0, 0), index(processActivities.size(), columnCount()));
}

/*
    returns -1 on failure
*/
int ProcessWidgetModel::getRowByPID(int pid)
{
    auto iter = processActivities.find(pid);
    if(iter == processActivities.end())
    {
        return -1;
    }
    return std::distance(processActivities.begin(), iter);
}

//------------------------------------------------/MODEL--------------------------------------


//------------------------------------------------PROXY--------------------------------------

ProcessWidgetProxyModel::ProcessWidgetProxyModel(QObject* parent)
    : QSortFilterProxyModel{parent} {}

bool ProcessWidgetProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left, ProcessWidgetModel::UserDataRole);
    QVariant rightData = sourceModel()->data(source_right, ProcessWidgetModel::UserDataRole);
    if(source_left.column() == ProcessWidgetModel::Name)
    {
        return leftData.value<QString>() < rightData.value<QString>();
    }
    else
    {
        return leftData.value<double>() < rightData.value<double>();
    }
}

//------------------------------------------------/PROXY--------------------------------------


