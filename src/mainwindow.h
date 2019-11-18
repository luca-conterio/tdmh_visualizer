#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphcontainer.h"
#include "logvisual.h"
#include "tsqueue.h"

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
    std::thread *textThread=nullptr;

    static void pollTextThread(TSQueue *tsq, LogVisual *lv);
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
    void setQueue(TSQueue *tsq);
signals:

public slots:
};

#endif // MAINWINDOW_H
