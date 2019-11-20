#include "linearea.h"
#include "logvisual.h"

#include <QWidget>
#include <QPainter>
#include <QTextBlock>

LogVisual::LogVisual(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineArea(this);
    //connect source, signal _> dest, member
    connect(this, &LogVisual::blockCountChanged, this, &LogVisual::updateLineNumberAreaWidth);
    connect(this, &LogVisual::updateRequest, this, &LogVisual::updateLineNumberArea);
    connect(this, &LogVisual::cursorPositionChanged, this, &LogVisual::highlightCurrentLine);
    connect(this, &LogVisual::lineAdded, this, &LogVisual::pushLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    this->setReadOnly(true);
}

int LogVisual::lineNumberAreaWidth()
{
    int digits = 1;
    int max = blockCount()>0?blockCount():1;
    const int scaleBy=10;
    while (max >= scaleBy) {
        max /= scaleBy;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void LogVisual::pushLine(const QString& line)
{
    this->insertPlainText(line);
}

void LogVisual::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);//Leave space for numbers
}


void LogVisual::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);//if change in y, scroll
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());//resize

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void LogVisual::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void LogVisual::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    //if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::cyan);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    //}

    setExtraSelections(extraSelections);
}

void LogVisual::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();

    int blockNumber = block.blockNumber()+1;

    //Top and bottom of a block(might be multiline in gui)
    int top = static_cast<int>( blockBoundingGeometry(block).translated(contentOffset()).top()  );
    int bottom = top +static_cast<int>( blockBoundingRect(block).height() );

    //while valid and visible at least partially before end
    while (block.isValid() && top <= event->rect().bottom()) {

        //if visible after the beginning
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber);
            painter.setPen(Qt::black);

            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>( blockBoundingRect(block).height() );
        ++blockNumber;
    }
}
