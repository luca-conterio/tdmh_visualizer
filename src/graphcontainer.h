#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include "configuration.h"
#include "graphcircle.h"
#include "logcontainer.h"
#include <QGraphicsView>
#include <memory>

/*!
 * \brief The GraphContainer class is the main widget of the graph drawing part of the GUI
 * \author Francesco Franzini
 */
class GraphContainer: public QGraphicsView
{
    Q_OBJECT

private:

    QGraphicsScene * const scene;

    //Vector to store circles objects
    std::vector<GraphCircle> circleVect;

    //Pointer to LogContainer used to fetch data
    std::shared_ptr<LogContainer> lC=nullptr;

    //Pointer to the arcs that have been drawn
    std::vector<QGraphicsLineItem*> lines;

    QPen weakPen;
    QPen strongPen;

    //true if the gui is in stat mode
    bool stat = false;

    const qreal dashCycleSize = 10.0;

    //zoom in and out with ctrl+'+' and ctrl+'-'
    float scaleFactor = 0.1;
    
public:

    /*!
     * \brief GraphContainer constructs a new View and scene
     * \param parent parent widget
     */
    GraphContainer(QWidget * parent=nullptr);

    /*!
     * \brief wheelEvent overrides standard wheel event to add cursor-centered zoom
     * \param event the QWheelEvent
     */
    void wheelEvent ( QWheelEvent * event ) override;

    /*!
     * \brief configGraph configures the window with the given settings
     * \param c the Configuration object
     * \param lC the log container pointer
     */
    void configGraph(const Configuration& c, const std::shared_ptr<LogContainer>& lC);

    void zoomIn();
    void zoomOut();

    void exportToPdf();

public slots:

    /*!
     * \brief updateGraph updates the graph to new LineN
     * \param lineN
     */
    void updateGraph(unsigned int lineN);
};

#endif // GRAPHCONTAINER_H
