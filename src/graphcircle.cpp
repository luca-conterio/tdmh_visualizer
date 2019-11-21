#include "graphcircle.h"
#include <QPainter>
#include <QGraphicsScene>

GraphCircle::GraphCircle(size_t i, int x, int y, QGraphicsScene * scene):x(x),y(y),i(i)
{
    this->t=new QGraphicsTextItem(QString::number(i));
    this->el=new QGraphicsEllipseItem(x-rad, y-rad, rad*2, rad*2);

    this->t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);

    el->setBrush(QBrush(Qt::red));
    t->setDefaultTextColor(QColor(Qt::darkGreen));
    scene->addItem(el);
    scene->addItem(t);
}

void GraphCircle::setPos(int x, int y)
{
    this->x=x;
    this->y=y;

    this->t->setPos(x-t->boundingRect().width()/2,y-t->boundingRect().height()/2);
    this->el->setRect(x-rad, y-rad, rad*2, rad*2);
}
