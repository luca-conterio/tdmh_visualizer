#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include "configuration.h"

#include <QGraphicsView>


/*!
 * \brief The GraphContainer class is the main widget of the graph drawing part of the GUI
 * \author Francesco Franzini
 */
class GraphContainer: public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene * scene;
    int _numScheduledScalings=0;
public:
    /*!
     * \brief GraphContainer constructs a new View and scene
     * \param parent parent widget
     */
    GraphContainer(QWidget * parent=nullptr);

    /*!
     * \brief wheelEvent overrides standard wheel event to add pointer-centered zoom
     * \param event the QWheelEvent
     */
    void wheelEvent ( QWheelEvent * event ) override;

    /*!
     * \brief configGraph configures the window with the given settings
     * \param c the Configuration object
     */
    void configGraph(Configuration c);

private slots:

signals:

public slots:

};

#endif // GRAPHCONTAINER_H
