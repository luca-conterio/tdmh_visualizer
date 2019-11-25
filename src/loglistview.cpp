#include "linearea.h"
#include "loglistview.h"
#include "stringlistmodel.h"
#include <iostream>
LogListView::LogListView(QWidget *parent):QListView(parent)
{
    this->lineNumberArea=new LineArea(this);

    updateLineNumberAreaWidth();
}

int LogListView::getSelectedLine()
{
    if(this->model()==nullptr||this->selectedIndexes().empty())return 0;
    return this->selectedIndexes().first().row();
}


int LogListView::lineNumberAreaWidth()
{
    if(this->model()==nullptr)return 0;
    int digits = 1;
    int fetched;
    fetched =this->model()->rowCount();

    int max = (fetched>0)?fetched:1;
    const int scaleBy=10;
    while (max >= scaleBy) {
        max /= scaleBy;
        ++digits;
    }
    //Space = 3+ digits*maximum digit size + a space separator each 3 characters
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits+digits/4);
    return space;
}


void LogListView::updateLineNumberAreaWidth()
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);//Leave space for numbers
}


void LogListView::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);//if change in y, scroll
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());//resize

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

void LogListView::verticalScrollbarValueChanged(int value)
{
    QListView::verticalScrollbarValueChanged(value);
    this->updateLineNumberArea(contentsRect(),value);
}

void LogListView::mousePressEvent(QMouseEvent *event)
{
    QListView::mousePressEvent(event);
    lineNumberArea->update();
}


void LogListView::resizeEvent(QResizeEvent *e)
{
    QListView::resizeEvent(e);
    QRect cr = contentsRect();
    updateLineNumberAreaWidth();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
