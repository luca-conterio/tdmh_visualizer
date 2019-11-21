#include "logvisual.h"
#include "mainwindow.h"

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <thread>
#include <QStatusBar>
#include <utility>
void MainWindow::pollTextThread(TSQueue *tsq, LogVisual *lv)
{
    //lv->pushLine(tsq->pop()+"\n");
    bool valid=true;
    const int linePerIter=1000;
    const int sleepTime=100;
    while(valid){
        for(int i=0;i<linePerIter &&valid;i++){
            emit lv->lineAdded(QString::fromStdString(tsq->pop(valid)+"\n"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),gCont(this)
{
    this->setWindowTitle("TDMH Log Visualizer");
    const double screenPercentage=0.7;
    resize(QDesktopWidget().availableGeometry(this).size() * screenPercentage);

    auto *layout = new QHBoxLayout;

    layout->addWidget(&lv);
    layout->addWidget(&gCont);
    centralW.setLayout(layout);

    this->setCentralWidget(&centralW);

    fileToolbar = addToolBar(tr("File"));
    //fileToolbar.addAction(newAct);
    //statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setQueue(TSQueue *tsq)
{
    if(textThread!=nullptr)return;
    this->ts=tsq;
    textThread=new std::thread(pollTextThread,tsq,&lv);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    if(ts!=nullptr)ts->breakQueue();
}

void MainWindow::setConfig(Configuration c)
{
    gCont.configGraph(std::move(c));
}
