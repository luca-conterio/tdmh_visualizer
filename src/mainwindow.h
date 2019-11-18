#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


/*!
 * \brief The MainWindow class is the main frame of the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief MainWindow Constructor
     * \param parent parent widget
     */
    MainWindow(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MAINWINDOW_H
