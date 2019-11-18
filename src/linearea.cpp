#include "linearea.h"



LineArea::LineArea(LogVisual *logV) : QWidget(logV) {
    this->logV=logV;
}

/*QSize LineArea::sizeHint() const {
    return QSize(logV->lineNumberAreaWidth(), 0);
}*/

void LineArea::paintEvent(QPaintEvent *event) {
    logV->lineNumberAreaPaintEvent(event);
}
