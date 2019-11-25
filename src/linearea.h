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
    void paintEvent(QPaintEvent *event) override;

private:
    LogListView * lV=nullptr;
};
#endif // LINEAREA_H
