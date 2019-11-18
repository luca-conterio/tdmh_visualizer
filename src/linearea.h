#ifndef LINEAREA_H
#define LINEAREA_H

#include "logvisual.h"

#include <QWidget>


/*!
 * \brief The LineArea class is the container for the line numbers on the GUI
 */
class LineArea : public QWidget
{
public:
    /*!
     * \brief LineArea builds a number column linked to a LogVisual parent
     * \param logV the LogVisual parent
     */
    LineArea(LogVisual *logV);

    /*!
     * \brief sizeHint returns a QSize object
     * \return the QSize object
     */
    //QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    LogVisual *logV;
};
#endif // LINEAREA_H
