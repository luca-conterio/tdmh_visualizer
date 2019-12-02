#include "graphcircle.h"
#include "graphcontainer.h"


#include <QTimeLine>
#include <QWheelEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <iostream>

GraphContainer::GraphContainer(QWidget *parent): QGraphicsView(parent),scene(new QGraphicsScene(this))
{
    this->setScene(scene);
    this->setBackgroundBrush(Qt::darkGray);


    weakPen.setStyle(Qt::DashDotLine);
    weakPen.setWidth(2);
    weakPen.setBrush(Qt::darkGreen);
    weakPen.setCapStyle(Qt::RoundCap);
    weakPen.setJoinStyle(Qt::RoundJoin);

    strongPen.setStyle(Qt::SolidLine);
    strongPen.setWidth(3);
    strongPen.setBrush(Qt::darkBlue);
    strongPen.setCapStyle(Qt::RoundCap);
    strongPen.setJoinStyle(Qt::RoundJoin);
}


void GraphContainer::wheelEvent(QWheelEvent* e) {

    if ((e->modifiers()&Qt::ControlModifier) == Qt::ControlModifier){//If control is pressed(otherwise it interferes with scrolling)
        QPoint  posAbsolute  = e->pos();
        QPointF posRelative = this->mapToScene(posAbsolute);
        double angle = e->angleDelta().y();

        double sc;
        const int fullAngle=360;
        const double scalingFactor=0.1; //Increase this to scroll faster

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
    const QString url = QString::fromStdString(c.getImgPath());
    QPixmap bgImg(url);

    QGraphicsPixmapItem pixItem(bgImg);
    scene->addPixmap(bgImg);

    scene->setSceneRect(bgImg.rect());//Fit image to scene

    const auto list=c.getNodeList();
    for(size_t i=0;i<list.size();i++){
        //GraphCircle constructor will take care of drawing the circles
        circleVect.emplace_back(i,list[i].first,list[i].second,scene);
    }

    stat=(c.getMode()==Configuration::STAT);
    std::cout<<"Configured graph with "<<circleVect.size()<<" elements"<<std::endl;

}

void GraphContainer::updateGraph(const unsigned int lineN)
{

    //Remove old lines
    for(auto line:lines){
        scene->removeItem(line);
        delete line;
    }
    lines.clear();

    if(stat){
        QPen pen=weakPen;

        //Pull matrix corresponding to selected line number
        const auto matrix=lC->getAvail(lineN<=lC->getTempThresh());
        const auto nodeCount=matrix.size();

        //Using only higher matrix
        for(size_t i=0;i<nodeCount;i++){
            for(size_t j=i;j<nodeCount;j++){
                auto* line=new QGraphicsLineItem(circleVect.at(i).getX(),circleVect.at(i).getY()
                                                 ,circleVect.at(j).getX(),circleVect.at(j).getY()
                                                 );

                QVector<qreal> dashes;
                qreal total=dashCycleSize;
                qreal length=(matrix[i][j])*total; //Percentage of total filled with line
                qreal space = total-length; //Remaining space is empty
                dashes << length<<space ;

                //Change color if timed or untimed
                if(lineN<=lC->getTempThresh())pen.setColor(Qt::blue);

                pen.setDashPattern(dashes);
                line->setPen(pen);

                this->scene->addItem(line);
                lines.push_back(line);
            }
        }
    }else{


        QPen pen;
        for(auto g1:circleVect){
            //Find outgoing arcs from each node
            const LogLine l=lC->findLine(g1.getI(),lineN);

            for(auto g2:circleVect){

                //If this node corrresponds to weak or strong arcs outgoing from g1
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
}


