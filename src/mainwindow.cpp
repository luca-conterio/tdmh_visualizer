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
#include <QLabel>

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
        for(int i = 0; i < linePerIter; i++) {
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
    QMainWindow(parent), centralW(new QWidget(this)), toolBar(new QToolBar(this)), 
    searchBox(new QLineEdit(this)), saveBox(new QLineEdit(this)),
    gCont(new GraphContainer(this)), listW(new LogListView(this)), model(new StringListModel(listW))
{

    this->setWindowTitle("TDMH Visualizer");
    this->setStyleSheet("QMainWindow {background: 'yellow';}");
    const double screenPercentage = 1.0; //0.8;
    resize(QDesktopWidget().availableGeometry(this).size() * screenPercentage);

    this->configureToolBar();

    this->configureViews();

    statusBar()->showMessage(tr("Ready"));

    // Keyboard shortcuts for network image zoom
    auto zoomInShortcut = new QShortcut(QKeySequence(tr("Ctrl++")), this);
    zoomInShortcut->setContext(Qt::ApplicationShortcut);
    connect(zoomInShortcut, &QShortcut::activated, gCont, &GraphContainer::zoomIn);

    auto zoomOutShortcut = new QShortcut(QKeySequence(tr("Ctrl+-")), this);
    zoomOutShortcut->setContext(Qt::ApplicationShortcut);
    connect(zoomOutShortcut, &QShortcut::activated, gCont, &GraphContainer::zoomOut);
}

void MainWindow::configureToolBar() 
{
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty->setMaximumWidth(30);
    toolBar->addWidget(empty);

    searchBox->setPlaceholderText("Go to line in log");
    searchBox->setMaximumWidth(400);
    auto searchShortcut = new QShortcut(QKeySequence(tr("return")), searchBox);
    searchShortcut->setContext(Qt::WidgetShortcut);
    connect(searchShortcut, &QShortcut::activated, this, &MainWindow::handleSearchLine);
    toolBar->addWidget(searchBox);
    QPushButton* searchButton = new QPushButton("Go", this);
    connect(searchButton, &QPushButton::released, this, &MainWindow::handleSearchLine);
    toolBar->addWidget(searchButton);

    QWidget* empty2 = new QWidget();
    empty2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty2->setMaximumWidth(1000);
    toolBar->addWidget(empty2);

    saveBox->setPlaceholderText("network.pdf");
    saveBox->setMaximumWidth(400);
    auto saveShortcut = new QShortcut(QKeySequence(tr("return")), saveBox);
    saveShortcut->setContext(Qt::WidgetShortcut);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::handleSavePDF);
    toolBar->addWidget(saveBox);
    QPushButton* saveButton = new QPushButton("Save PDF", this);
    connect(saveButton, &QPushButton::released, this, &MainWindow::handleSavePDF);
    toolBar->addWidget(saveButton);

    QWidget* empty3 = new QWidget();
    empty3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty3->setMaximumWidth(1000);
    toolBar->addWidget(empty3);

    QLabel* zoomLabel = new QLabel("Zoom: ", this);
    toolBar->addWidget(zoomLabel);
    QPushButton* zoomInButton = new QPushButton("+", this);
    zoomInButton->setMaximumWidth(50);
    toolBar->addWidget(zoomInButton);
    connect(zoomInButton, &QPushButton::released, gCont, &GraphContainer::zoomIn);
    QPushButton* zoomOutButton = new QPushButton("-", this);
    zoomOutButton->setMaximumWidth(50);
    toolBar->addWidget(zoomOutButton);
    connect(zoomOutButton, &QPushButton::released, gCont, &GraphContainer::zoomOut);

    QWidget* empty4 = new QWidget();
    empty4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty4->setMaximumWidth(30);
    toolBar->addWidget(empty4);

    this->addToolBar(toolBar);

    // Keyboard shortcuts for line search and image save
    auto searchRowShortcut = new QShortcut(QKeySequence(tr("Ctrl+f")), this);
    searchRowShortcut->setContext(Qt::ApplicationShortcut);
    connect(searchRowShortcut, &QShortcut::activated, this, &MainWindow::focusSearchBox);

    auto saveImageShortcut = new QShortcut(QKeySequence(tr("Ctrl+s")), this);
    saveImageShortcut->setContext(Qt::ApplicationShortcut);
    connect(saveImageShortcut, &QShortcut::activated, this, &MainWindow::focusSaveBox);
}

void MainWindow::configureViews()
{
    auto * const mainWindowLayout = new QHBoxLayout;
    auto * const hSplitter = new QSplitter(Qt::Horizontal);

    //Init view
    listW->setUniformItemSizes(true);

    //Init model and link to view
    listW->setModel(model);
    connect(listW->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          this, SLOT(selectionChanged(QItemSelection)));
    connect(model, &StringListModel::fetched, listW, &LogListView::updateLineNumberAreaWidth);

    //Add to a splitter left container and right graph container
    hSplitter->addWidget(listW);
    hSplitter->addWidget(gCont);
    //hSplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
    auto width = QDesktopWidget().availableGeometry(this).width();
    hSplitter->setSizes(QList<int>({width*42/100, width*58/100}));

    //Set splitter as main widget
    mainWindowLayout->addWidget(hSplitter);
    centralW->setLayout(mainWindowLayout);
    this->setCentralWidget(centralW);
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

void MainWindow::handleSearchLine() 
{
    const auto rowNumber = searchBox->text().toInt();
    listW->goToLine(rowNumber);

    //QString previousStatusMessage(statusMessage); // save current status bar message
    /*if (rowNumber == 0) {
        std::cout << "Go to log beginning" << std::endl;
        showStatusMessage("Go to log beginning");
    }
    else {
        std::cout << "Go to log line " << rowNumber << std::endl;
        showStatusMessage("Go to log line " + QString::number(rowNumber));
    }*/
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //showStatusMessage(previousStatusMessage); // set previous status bar message
}

void MainWindow::handleSavePDF() 
{
    auto filename = saveBox->text();
    if (filename == "") {
        filename = QString("network.pdf");
    }
    
    gCont->exportToPDF(filename);
    
    //QString previousStatusMessage(statusMessage); // save current status bar message
    std::cout << "Exported network image: " << filename.toStdString() << std::endl;
    showStatusMessage("Exported network image: " + filename); // + " to PDF");
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    //showStatusMessage(previousStatusMessage); // set previous status bar message
}

void MainWindow::focusSearchBox()
{
    searchBox->setFocus();
}

void MainWindow::focusSaveBox()
{
    saveBox->setFocus();
}