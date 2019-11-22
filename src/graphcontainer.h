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
    QGraphicsScene * scene;
    int _numScheduledScalings=0;
    std::vector<GraphCircle> circleVect;
    std::shared_ptr<LogContainer> lC=nullptr;
    std::vector<QGraphicsLineItem*> lines;
    QPen weakPen;
    QPen strongPen;

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
     * \param lC the log container pointer
     */
    void configGraph(const Configuration& c, const std::shared_ptr<LogContainer>& lC);



public slots:
    /*!
     * \brief updateGraph updates the graph to new LineN
     * \param lineN
     */
    void updateGraph(unsigned int lineN);
signals:
};

#endif // GRAPHCONTAINER_H
