#include "graphcircle.h"
#include "graphcontainer.h"


#include <QTimeLine>
#include <QWheelEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include <iostream>
GraphContainer::GraphContainer(QWidget *parent): QGraphicsView(parent)
{
    scene=new QGraphicsScene(this);
    this->setScene(scene);
    this->setBackgroundBrush(Qt::darkGray);


    weakPen.setStyle(Qt::DashDotLine);
    weakPen.setWidth(2);
    weakPen.setBrush(Qt::green);
    weakPen.setCapStyle(Qt::RoundCap);
    weakPen.setJoinStyle(Qt::RoundJoin);

    strongPen.setStyle(Qt::SolidLine);
    strongPen.setWidth(3);
    strongPen.setBrush(Qt::darkBlue);
    strongPen.setCapStyle(Qt::RoundCap);
    strongPen.setJoinStyle(Qt::RoundJoin);
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

void GraphContainer::configGraph(const Configuration& c, const std::shared_ptr<LogContainer>& lC)
{
    this->lC=lC;
    QString url = QString::fromStdString(c.getImgPath());
    QPixmap bgImg(url);


    //bgImg=bgImg.scaled(viewport()->contentsRect().size());
    QGraphicsPixmapItem pixItem(bgImg);
    scene->addPixmap(bgImg);

    scene->setSceneRect(bgImg.rect());

    auto list=c.getNodeList();
    for(size_t i=0;i<list.size();i++){
        /*scene->addEllipse(list[i].first-rad, list[i].second-rad, rad*2.0, rad*2.0,
                    QPen(), QBrush(Qt::red));*/
        circleVect.emplace_back(i,list[i].first,list[i].second,scene);
    }
    std::cout<<"Configured graph with "<<circleVect.size()<<" elements"<<std::endl;

}

void GraphContainer::updateGraph(unsigned int lineN)
{
    for(auto line:lines){
        scene->removeItem(line);
        delete line;
    }
    lines.clear();

    QPen pen;
    for(auto g1:circleVect){
        LogLine l=lC->findLine(g1.getI(),lineN);
        for(auto g2:circleVect){
            if(l.getStrongMask(g2.getI())){
                pen=strongPen;
            }else if(l.getWeakMask(g2.getI())){
                pen=weakPen;
            }else{
                continue;
            }

            auto* line=new QGraphicsLineItem(g1.getX(),g1.getY(),g2.getX(),g2.getY());
            line->setPen(pen);
            this->scene->addItem(line);
            lines.push_back(line);

        }

    }
}


