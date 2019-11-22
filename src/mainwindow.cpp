#include "logvisual.h"
#include "mainwindow.h"

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <thread>
#include <QStatusBar>
#include <utility>
#include <QWidget>
#include <QSplitter>
void MainWindow::pollTextThread(const std::shared_ptr<TSQueue>& tsq, LogVisual *lv,MainWindow* parent)
{
    /*bool valid=true;
    const int linePerIter=600;
    const int sleepTime=100;
    while(valid){
        for(int i=0;i<linePerIter &&valid;i++){
            emit lv->lineAdded(QString::fromStdString(tsq->pop(valid)+"\n"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }*/

    parent->showStatusMessage("Reading in batchmode");
    bool valid=true;
    const int linePerIter=10000;
    const int sleepTime=100;
    std::string buf;
    QString str("");
    while(valid&&tsq->isBatch()){
        for(int i=0;i<linePerIter;i++){
            buf=tsq->pop(valid);
            if(!valid){
                if(!tsq->isBroken()){
                    valid=true;
                }
                break;
            }
            str.append(QString::fromStdString(buf+"\n"));
        }
        emit lv->lineAdded(str);
        str.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
    parent->showStatusMessage("Reading in realtime mode");
    valid=true;
    while(valid){
        for(int i=0;i<linePerIter &&valid;i++){
            buf=tsq->pop(valid);
            if(!valid){
                if(!tsq->isBroken()){
                    valid=true;
                }
                break;
            }
            emit lv->lineAdded(QString::fromStdString(buf+"\n"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
    parent->showStatusMessage("Done parsing input");

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    gCont=new GraphContainer(this);
    this->setWindowTitle("TDMH Log Visualizer");
    const double screenPercentage=0.7;
    resize(QDesktopWidget().availableGeometry(this).size() * screenPercentage);

    auto *layout2 = new QHBoxLayout;
    auto * layout=new QSplitter(Qt::Horizontal);

    lv=new LogVisual(this);
    layout->addWidget(lv);
    layout->addWidget(gCont);
    layout->setSizes(QList<int>({INT_MAX, INT_MAX}));
    layout2->addWidget(layout);
    centralW.setLayout(layout2);
    this->setCentralWidget(&centralW);

    fileToolbar = addToolBar(tr("File"));
    //fileToolbar.addAction(newAct);
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setQueue(const std::shared_ptr<TSQueue>& tsq)
{
    if(textThread!=nullptr)return;
    this->ts=tsq;
    textThread=std::make_unique<std::thread>(pollTextThread,tsq,lv,this);
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
    connect(lv, &LogVisual::cursorChanged, gCont, &GraphContainer::updateGraph);
    lv->makeReady();
}

void MainWindow::showStatusMessage(QString str)
{
    statusBar()->showMessage(str);
}
