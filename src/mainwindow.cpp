#include "linearea.h"
#include "mainwindow.h"

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <thread>
#include <QStatusBar>
#include <utility>
#include <QWidget>
#include <QSplitter>
#include <QStringListModel>
#include <QCoreApplication>

void MainWindow::pollTextThread(const std::shared_ptr<TSQueue>& tsq,MainWindow* parent,int linePerIter,int sleepTime)
{
    parent->showStatusMessage("Reading in batchmode");
    bool valid=true;
    std::string *buf;
    std::vector<std::string*> str;
    while(valid&&tsq->isBatch()){
        for(int i=0;i<linePerIter;i++){
            buf=tsq->pop(valid,false);
            if(!valid){
                if(!tsq->isBroken()){
                    valid=true;
                }
                break;
            }
            str.push_back(buf);
        }
        if(!str.empty()){
            emit parent->newLineAvailable(str);
            str.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
    parent->showStatusMessage("Reading in realtime mode");
    parent->model->goRealTime();
    valid=true;
    while(valid){
        for(int i=0;i<linePerIter;i++){
            buf=tsq->pop(valid,false);
            if(!valid){
                if(!tsq->isBroken()){
                    valid=true;
                }
                break;
            }
            str.push_back(buf);
        }
        if(!str.empty()){
            emit parent->newLineAvailable(str);
            str.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        /*buf=tsq->pop(valid,true);
        if(!valid){
            if(!tsq->isBroken()){
                valid=true;
            }
            continue;
        }
        str.push_back(buf);
        emit parent->newLineAvailable(str);
        str.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime*5));*/
    }
    parent->showStatusMessage("Done parsing input");

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    gCont=new GraphContainer(this);
    this->setWindowTitle("TDMH Log Visualizer");
    const double screenPercentage=0.7;
    resize(QDesktopWidget().availableGeometry(this).size() * screenPercentage);

    auto *mainWindowLayout = new QHBoxLayout;
    auto * splitter=new QSplitter(Qt::Horizontal);

    //Init view
    listW=new LogListView(this);
    listW->setUniformItemSizes(true);

    //Init model and link to view
    model=new StringListModel(listW);
    listW->setModel(model);
    connect(listW->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(selectionChanged(QItemSelection)));
    connect(model, &StringListModel::fetched, listW,&LogListView::updateLineNumberAreaWidth);

    //Add to a splitter left container and right graph container
    splitter->addWidget(listW);
    splitter->addWidget(gCont);
    splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    //Set splitter as main widget
    mainWindowLayout->addWidget(splitter);
    centralW.setLayout(mainWindowLayout);
    this->setCentralWidget(&centralW);

    fileToolbar = addToolBar(tr("File"));
    //fileToolbar.addAction(newAct);
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setQueue(const std::shared_ptr<TSQueue>& tsq)
{
    if(textThread!=nullptr)return;
    this->ts=tsq;
    if(c.getBatchFirst()){
        model->disableBatchUpdates();
        linePerIter=INT_MAX;
        sleepTime=sleepTime*2;
    }
    textThread=std::make_unique<std::thread>(pollTextThread,tsq,this,linePerIter,sleepTime);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    if(ts!=nullptr)ts->breakQueue();
    textThread->detach();//TODO better than this
    lld->stop();
}

void MainWindow::setConfig(const Configuration& c, const std::shared_ptr<LogContainer> &gC, std::shared_ptr<LogLoader> lld)
{

    gCont->configGraph(c,gC);
    this->lld=std::move(lld);
    qRegisterMetaType<std::vector<std::string*>>("std::vector<std::string*>");
    connect(this, &MainWindow::newLineAvailable, model, &StringListModel::addString);
    this->c=c;
}

void MainWindow::showStatusMessage(const QString& str)
{
    statusBar()->showMessage(str);
}

void MainWindow::selectionChanged(const QItemSelection& selection)
{
    gCont->updateGraph(static_cast<unsigned int>(
                           selection.indexes().first().row()
                           ));
}
