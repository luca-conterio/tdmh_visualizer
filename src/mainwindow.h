#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphcontainer.h"
#include "loglistview.h"
#include "logloader.h"
#include "stringlistmodel.h"
#include "tsqueue.h"
#include <memory>
#include <QMainWindow>
#include <thread>
#include <QListWidget>

Q_DECLARE_METATYPE( std::vector<std::string*> )
/*!
 * \brief The MainWindow class is the main window of the log viewer
 * \author Francesco Franzini
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget centralW;
    QToolBar *fileToolbar=nullptr;
    GraphContainer *gCont=nullptr;
    LogListView *listW=nullptr;
    StringListModel *model=nullptr;
    std::unique_ptr<std::thread> textThread=nullptr;
    std::shared_ptr<TSQueue> ts=nullptr;
    std::shared_ptr<LogLoader> lld=nullptr;
    static void pollTextThread(const std::shared_ptr<TSQueue>&tsq, MainWindow *parent);
public:
    /*!
     * \brief MainWindow Constructor
     * \param parent parent widget
     */
    MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief setQueue sets the queue where the text will be polled from and starts the thread
     * \param tsq the ts queue
     */
    void setQueue(const std::shared_ptr<TSQueue>&);

    /*!
     * \brief closeEvent extends the inherited method to break the pipe
     * \param event QCloseEvent
     */
    void closeEvent (QCloseEvent *event) override;

    /*!
     * \brief setConfig sets the configuration and loader to be used
     * \param c the configuration
     * \param gC the GraphContainer pointer
     * \param lld pointer to the loader
     */
    void setConfig(const Configuration& c, const std::shared_ptr<LogContainer>& gC, std::shared_ptr<LogLoader> lld);

    /*!
     * \brief showStatusMessage shows a message in the status bar
     * \param str message to be sent
     */
    void showStatusMessage(const QString& str);

signals:

    /*!
     * \brief newLineAvailable signal to be emitted when a new vector of lines is ready
     * \param str the vector of lines
     */
    void newLineAvailable(std::vector<std::string*> str);
public slots:

    /*!
     * \brief selectionChanged catches selectionChanged signal from the view and updates the graph
     * \param selection current selection
     */
    void selectionChanged(const QItemSelection& selection);
};

#endif // MAINWINDOW_H
