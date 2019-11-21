#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphcontainer.h"
#include "logloader.h"
#include "logvisual.h"
#include "tsqueue.h"
#include <memory>
#include <QMainWindow>
#include <QToolBar>
#include <thread>


/*!
 * \brief The MainWindow class is the main frame of the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    LogVisual lv;
    QWidget centralW;
    QToolBar *fileToolbar;
    GraphContainer gCont;
    std::unique_ptr<std::thread> textThread=nullptr;
    std::shared_ptr<TSQueue> ts=nullptr;
    std::shared_ptr<LogLoader> lld=nullptr;
    static void pollTextThread(const std::shared_ptr<TSQueue>&tsq, LogVisual *lv);
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
     * \param lld pointer to the loader
     */
    void setConfig(const Configuration& c,std::shared_ptr<LogLoader> lld);

signals:

public slots:
};

#endif // MAINWINDOW_H
