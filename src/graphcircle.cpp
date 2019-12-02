#include "graphcircle.h"
#include <QPainter>
#include <QGraphicsScene>

int GraphCircle::getX() const
{
    return x;
}

int GraphCircle::getY() const
{
    return y;
}

unsigned int GraphCircle::getI() const
{
    return static_cast<unsigned int>(i);
}

GraphCircle::GraphCircle(const size_t i, const int x, const int y, QGraphicsScene *scene):x(x),y(y),i(i)
{
    this->t=new QGraphicsTextItem(QString::number(i));//Text
    this->el=new QGraphicsEllipseItem(x-rad, y-rad, rad*2, rad*2);//Circle

    this->t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);

    el->setBrush(QBrush(Qt::red));
    t->setDefaultTextColor(QColor(Qt::black));

    el->setZValue(1);//Bring forward
    t->setZValue(1);

    scene->addItem(el);
    scene->addItem(t);

}

/*void GraphCircle::setPos(int x, int y)
{
    this->x=x;
    this->y=y;

    this->t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);
    this->el->setRect(x-rad, y-rad, rad*2, rad*2);
}*/
