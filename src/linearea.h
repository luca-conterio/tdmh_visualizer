#ifndef LINEAREA_H
#define LINEAREA_H

#include "logvisual.h"

#include <QWidget>



class LineArea : public QWidget
{
public:
    LineArea(LogVisual *logV) : QWidget(logV) {
        this->logV=logV;
    }

    QSize sizeHint() const override {
        return QSize(logV->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        logV->lineNumberAreaPaintEvent(event);
    }

private:
    LogVisual *logV;
};
#endif // LINEAREA_H
