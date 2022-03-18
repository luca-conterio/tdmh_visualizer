#include "linearea.h"

#include <QPainter>
#include <QModelIndex>
#include <iostream>
#include <QPaintEvent>

LineArea::LineArea(LogListView *parent) : QWidget(parent), lV(parent) {
}


void LineArea::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    painter.fillRect(event->rect(), Qt::lightGray); //BackGround color

    QModelIndex ind=lV->indexAt(lV->rect().topLeft());
    int blockNumber = ind.row(); //Index of first visible block

    //Top= top of the view - the margin to the text
    int top = static_cast<int>(lV->contentsRect().top() - lV->contentsMargins().top());

    //Bottom = top+ row size
    int bottom = top + static_cast<int>(lV->sizeHintForRow(blockNumber));

    //while valid and visible at least partially before end
    while (ind.isValid() && top <= event->rect().bottom()) {
        //if visible after the beginning
        if (bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber);

            //Insert spaces to ease reading of the line number
            for(int i = number.size() - 3; i > 0; i -= 3) {
                number.insert(i, ' ');
            }

            //Highlight selected line number
            if(blockNumber != lV->getSelectedLine()) {
                painter.setPen(Qt::black);
            }else{
                painter.setPen(Qt::red);
            }

            painter.drawText(0, top, this->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        //Move variables to next row
        top = bottom;
        ++blockNumber;
        bottom = top + static_cast<int>(lV->sizeHintForRow(blockNumber));
        ind=lV->model()->index(blockNumber, 0);

    }
}
