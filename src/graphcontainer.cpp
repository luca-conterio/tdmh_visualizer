#include "graphcontainer.h"


#include <QTimeLine>
#include <QWheelEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
GraphContainer::GraphContainer(QWidget *parent): QGraphicsView(parent)
{
    scene=new QGraphicsScene(this);
    scene->addText("Hello, world!");
    this->setScene(scene);
    this->setStyleSheet("QGraphicsView {background: 'green';}");
}


void GraphContainer::wheelEvent(QWheelEvent* e) {

    if ((e->modifiers()&Qt::ControlModifier) == Qt::ControlModifier){
        QPoint  posAbsolute  = e->pos();
        QPointF posRelative = this->mapToScene(posAbsolute);
        double angle = e->angleDelta().y();

        double sc;
        const int fullAngle=360;
        const double scalingFactor=0.1;
        sc= 1+ (angle/fullAngle *scalingFactor);
        this->scale(sc, sc);

        double w = this->viewport()->width();
        double h = this->viewport()->height();
        //W and H scaled to scene

        double wrel = this->mapToScene(QPoint(static_cast<int>(w)-1, 0)).x()-this->mapToScene(QPoint(0,0)).x();
        double hrel = this->mapToScene(QPoint(0, static_cast<int>(h)-1)).y()-this->mapToScene(QPoint(0,0)).y();

        double lf = posRelative.x() - posAbsolute.x() * wrel / w;
        double tf = posRelative.y() - posAbsolute.y() * hrel / h;

        this->ensureVisible(lf, tf, wrel, hrel, 0, 0);//set viewport

        QPointF newPos = this->mapToScene(posAbsolute);


        this->ensureVisible(QRectF(QPointF(lf, tf) - newPos + posRelative,
                                   QSizeF(wrel, hrel)), 0, 0);

        e->accept();
    }else{
        QGraphicsView::wheelEvent(e);
    }
}

void GraphContainer::configGraph(Configuration c)
{
    QString url = QString::fromStdString(c.getImgPath());
    QPixmap bgImg(url);

    bgImg=bgImg.scaled(viewport()->contentsRect().size());
    QGraphicsPixmapItem pixItem(bgImg);
    scene->addPixmap(bgImg);
}

