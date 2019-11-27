#ifndef RTWATCHER_H
#define RTWATCHER_H

/*#include <QObject>
#include <QThread>


class RTWatcher : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter) {
        QString result;
        // ... here is the expensive or blocking operation ...
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() {
        RTWatcher *worker = new RTWatcher;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &RTWatcher::doWork);
        connect(worker, &RTWatcher::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);
signals:
    void operate(const QString &);
};
*/
#endif // RTWATCHER_H
