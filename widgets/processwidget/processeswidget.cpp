#include "processeswidget.hpp"

ProcessesWidget::ProcessesWidget(InfoManager::pointer _infoManager, QWidget *parent)
    : EmulateableWidget(parent), infoManager{_infoManager}
{
    setUpdateInterval(1000);
    createWidgets();
    createLayout();
    makeConnections();
}

ProcessActivities ProcessesWidget::getActivitiesOfPID(PID pid)
{
    return processesModel->getProcessActivities()[pid];
}

void ProcessesWidget::createWidgets()
{
    QHash<PID, ProcessActivities> initActivities = unorderedMapToQHash(infoManager->getActivitiesMap());
    processesModel = new ProcessWidgetModel(initActivities);
    sortProxyModel = new ProcessWidgetProxyModel;
    sortProxyModel->setSourceModel(processesModel);
    processesView = new QTableView;
    processesView->setModel(sortProxyModel);
    processesView->setSortingEnabled(true);
    processesView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    processesView->setContextMenuPolicy(Qt::CustomContextMenu);
    processesView->setSelectionBehavior(QAbstractItemView::SelectRows);
    for(int i = 0; i < processesView->horizontalHeader()->count(); ++i)
    {
        // all columns are sized by contents, except last - it is stretched
        if(i != processesView->horizontalHeader()->count() - 1)
        {
            processesView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }
        else
        {
            processesView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
    }
}

void ProcessesWidget::createLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(processesView);
    setLayout(mainLayout);
}

void ProcessesWidget::makeConnections()
{
    // we are calculating PID value here to prevent connection delay(if it has place?)
    connect(processesView, &QTableView::customContextMenuRequested, [this](QPoint point){
        QModelIndex clickedIndexPID = sortProxyModel->index(processesView->indexAt(point).row(), processesModel->PID);
        showProcessContextMenu(point, sortProxyModel->data(clickedIndexPID).value<PID>());
    });
}

void ProcessesWidget::setDataPrecision(int prec)
{
    processesModel->setDataPrecision(prec);
}

void ProcessesWidget::setChartGridEnabled(bool)
{

}

void ProcessesWidget::setChartMode(Modes)
{

}

void ProcessesWidget::start()
{
    try
    {
        updater();
        EmulateableWidget::start();
    }
    catch (...)
    {
        qWarning() << "Processes widget is not working";
    }
}

void ProcessesWidget::updater()
{
    QHash<PID, ProcessActivities> curActivities = unorderedMapToQHash(infoManager->getActivitiesMap());
    processesModel->clearAndSetActivitiesMap(std::move(curActivities));
}

void ProcessesWidget::showProcessContextMenu(QPoint point, PID pid)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction killAction(tr("Kill"), this);
    connect(&killAction, &QAction::triggered, [this, pid](){
        killProcess(pid);
    });
    contextMenu.addAction(&killAction);

    QAction terminateAction(tr("Terminate"), this);
    connect(&terminateAction, &QAction::triggered, [this, pid](){
        terminateProcess(pid);
    });
    contextMenu.addAction(&terminateAction);

    contextMenu.exec(mapToGlobal(point));
}

void ProcessesWidget::killProcess(PID pid)
{
    if(!SystemGetter::getSystem()->killProcess(pid))
    {
        QMessageBox::warning(this, tr("Process killing error"), tr("Cannot kill this process"));
    }
}

void ProcessesWidget::terminateProcess(PID pid)
{
    if(!SystemGetter::getSystem()->terminateProcess(pid))
    {
        QMessageBox::warning(this, tr("Process terminating error"), tr("Cannot terminate this process"));
    }
}
