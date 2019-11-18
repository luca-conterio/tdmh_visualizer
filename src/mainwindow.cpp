#include "logvisual.h"
#include "mainwindow.h"

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <thread>
void MainWindow::pollTextThread(TSQueue *tsq, LogVisual *lv)
{
    //lv->pushLine(tsq->pop()+"\n");
    while(!tsq->isBroken()){
        for(int i=0;i<1000;i++){
            emit lv->lineAdded(QString::fromStdString(tsq->pop()+"\n"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)//, lv(this)
{
    this->setWindowTitle("TDMH Log Visualizer");
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(&lv);
    layout->addWidget(&gCont);
    centralW.setLayout(layout);

    this->setCentralWidget(&centralW);

    fileToolbar = addToolBar(tr("File"));
    //fileToolbar.addAction(newAct);
}

void MainWindow::setQueue(TSQueue *tsq)
{
    if(textThread!=nullptr)return;
    textThread=new std::thread(pollTextThread,tsq,&lv);
}
