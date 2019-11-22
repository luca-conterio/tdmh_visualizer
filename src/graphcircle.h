#ifndef GRAPHCIRCLE_H
#define GRAPHCIRCLE_H
#include <QGraphicsItem>
#include <QWidget>

/*!
 * \brief The GraphCircle class contains the representation of a circle and text on the map
 * \author Francesco Franzini
 */
class GraphCircle
{
private:
    int x;
    int y;
    const size_t i;
    QGraphicsEllipseItem * el;
    QGraphicsTextItem * t;
    static const int rad=13;
public:

    /*!
     * \brief GraphCircle initializes this object with the given settings and adds them to the scene
     * \param i index to write on screen
     * \param x x position
     * \param y y position
     * \param scene where to draw
     */
    GraphCircle(size_t i, int x, int y, QGraphicsScene *scene);

    /*!
     * \brief setPos changes the position
     * \param x new x
     * \param y new y
     */
    void setPos(int x,int y);

    int getX() const;
    int getY() const;
    unsigned int getI() const;
};

#endif // GRAPHCIRCLE_H
