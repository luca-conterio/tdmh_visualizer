#include "linearea.h"
#include "loglistview.h"
#include "stringlistmodel.h"
#include <iostream>

LogListView::LogListView(QWidget *parent):QListView(parent), lineNumberArea(new LineArea(this))
{
    updateLineNumberAreaWidth();
}

int LogListView::getSelectedLine() const
{
    if(this->model()==nullptr || this->selectedIndexes().empty() )return 0;
    return this->selectedIndexes().first().row();
}


int LogListView::lineNumberAreaWidth() const
{
    if(this->model()==nullptr)return 0;
    int digits = 1;
    const auto fetched =this->model()->rowCount();//gui row count

    int max = (fetched>0)?fetched:1;
    const int scaleBy=10;
    while (max >= scaleBy) {//number of digits is how many times it can be divided by 10 +1
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


void LogListView::updateLineNumberArea(const QRect &rect, const int dy)
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
