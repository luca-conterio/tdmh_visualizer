#ifndef LINEAREA_H
#define LINEAREA_H

#include "loglistview.h"

#include <QWidget>


/*!
 * \brief The LineArea class is the container for the line numbers on the GUI
 * \author Francesco Franzini
 */
class LineArea : public QWidget
{
public:
    /*!
     * \brief LineArea builds a number column linked to a LogListView parent
     * \param parent the LogListView parent
     */
    LineArea(LogListView *parent);

protected:

    /*!
     * \brief paintEvent paints the line numbers of the visible lines
     * \param event the paint event
     */
    void paintEvent(QPaintEvent *event) override;

private:

    //Parent pointer without casting to generic QWidget
    LogListView * const lV=nullptr;
};
#endif // LINEAREA_H
