#include "linearea.h"
#include "mainwindow.h"

#include <QDesktopWidget>
#include <QHBoxLayout>
#include <thread>
#include <QStatusBar>
#include <utility>
#include <QWidget>
#include <QSplitter>
#include <QStringListModel>
#include <QCoreApplication>
#include <QShortcut>

void MainWindow::pollTextThread(const std::shared_ptr<TSQueue>& tsq, MainWindow *parent,const int linePerIter,const int sleepTime)
{
    parent->showStatusMessage("Reading in batch mode");
    bool valid = true;
    std::string *buf;
    std::vector<std::string*> str;

    while(valid && tsq->isBatch()){
        for(int i = 0; i < linePerIter; i++){
            buf = tsq->pop(valid, false);
            if(!valid) {
                if(!tsq->isBroken()) { //if hit empty queue, keep going
                    valid = true;
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
    valid = true;

    while(valid) {
        for(int i = 0; i < linePerIter; i++){
            buf = tsq->pop(valid, false);
            if(!valid) {
                if(!tsq->isBroken()) {
                    valid = true;
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
    parent->showStatusMessage("Done parsing input");

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), centralW(new QWidget(this)),gCont(new GraphContainer(this)),listW(new LogListView(this)),model(new StringListModel(listW))
{

    this->setWindowTitle("TDMH Visualizer");
    this->setStyleSheet("QMainWindow {background: 'yellow';}");
    const double screenPercentage = 0.8;
    resize(QDesktopWidget().availableGeometry(this).size() * screenPercentage);

    auto * const mainWindowLayout = new QHBoxLayout;
    auto * const splitter = new QSplitter(Qt::Horizontal);

    //Init view
    listW->setUniformItemSizes(true);

    //Init model and link to view
    listW->setModel(model);
    connect(listW->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(selectionChanged(QItemSelection)));
    connect(model, &StringListModel::fetched, listW, &LogListView::updateLineNumberAreaWidth);

    //Add to a splitter left container and right graph container
    splitter->addWidget(listW);
    splitter->addWidget(gCont);
    //splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
    auto width = QDesktopWidget().availableGeometry(this).width();
    splitter->setSizes(QList<int>({width*45/100, width*55/100}));

    //Set splitter as main widget
    mainWindowLayout->addWidget(splitter);
    centralW->setLayout(mainWindowLayout);
    this->setCentralWidget(centralW);

    statusBar()->showMessage(tr("Ready"));

    // Keyboard shortcuts
    auto searchRowShortcut = new QShortcut(QKeySequence(tr("Ctrl+f")), this);
    searchRowShortcut->setContext(Qt::ApplicationShortcut);
    connect(searchRowShortcut, &QShortcut::activated, listW, &LogListView::searchActivated);

    auto zoomInShortcut = new QShortcut(QKeySequence(tr("Ctrl++")), this);
    zoomInShortcut->setContext(Qt::ApplicationShortcut);
    connect(zoomInShortcut, &QShortcut::activated, gCont, &GraphContainer::zoomIn);

    auto zoomOutShortcut = new QShortcut(QKeySequence(tr("Ctrl+-")), this);
    zoomOutShortcut->setContext(Qt::ApplicationShortcut);
    connect(zoomOutShortcut, &QShortcut::activated, gCont, &GraphContainer::zoomOut);

    auto saveImageShortcut = new QShortcut(QKeySequence(tr("Ctrl+s")), this);
    saveImageShortcut->setContext(Qt::ApplicationShortcut);
    connect(saveImageShortcut, &QShortcut::activated, this, &MainWindow::exportToPdf);
}

void MainWindow::setQueue(const std::shared_ptr<TSQueue>& tsq)
{
    if (textThread != nullptr) return;
    this->ts = tsq;
    if(c.getBatchFirst() || c.getMode() == Configuration::STAT) {
        model->disableBatchUpdates();
        linePerIter = INT_MAX;
        sleepTime = sleepTime * 2;
    }
    textThread=std::make_unique<std::thread>(pollTextThread, tsq, this, linePerIter, sleepTime);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    if (ts != nullptr) ts->breakQueue();
    textThread->detach();
    lld->stop();
}

void MainWindow::setConfig(const Configuration& c, const std::shared_ptr<LogContainer>& gC, std::shared_ptr<LogLoader> lld,const std::shared_ptr<TSQueue>& tsq)
{

    gCont->configGraph(c,gC);
    this->lld = std::move(lld);
    qRegisterMetaType<std::vector<std::string*>>("std::vector<std::string*>");
    connect(this, &MainWindow::newLineAvailable, model, &StringListModel::addString);
    this->c = c;
    this->setQueue(tsq);
}

void MainWindow::showStatusMessage(const QString& str)
{
    statusMessage = QString(str);
    statusBar()->showMessage(str);
}

void MainWindow::selectionChanged(const QItemSelection& selection)
{
    gCont->updateGraph(static_cast<unsigned int>(selection.indexes().first().row()));
}

void MainWindow::exportToPdf() 
{
    gCont->exportToPdf();
    QString previousStatusMessage(statusMessage); // save current status bar message
    showStatusMessage("Exported image to PDF");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    showStatusMessage(previousStatusMessage); // set previous status bar message
}
