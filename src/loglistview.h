#ifndef LOGLISTVIEW_H
#define LOGLISTVIEW_H

#include <QListView>


/*!
 * \brief The LogListView class adds a line number column to a QListView
 * \author Francesco Franzini
 */
class LogListView: public QListView
{
    Q_OBJECT

public:

    /*!
     * \brief LogListView initializes this LogListView and adds a line number column
     * \param parent parent widget
     */
    LogListView(QWidget *parent);

    /*!
     * \brief getSelectedLine returns the row number of the currently selected line, 0 if none selected
     * \return row number of currently selected line
     */
    int getSelectedLine() const;



private:
    QWidget * const lineNumberArea;
    int lineNumberAreaWidth() const;
    void updateLineNumberArea(const QRect &rect,int dy);

public slots:

    /*!
     * \brief updateLineNumberAreaWidth updates the margins of the Text part of this widget
     */
    void updateLineNumberAreaWidth();

    /*!
     * \brief goToFindLine scrolls to the line number indicated in the find box
     */
    void goToLine(int rowNumber);

    // QAbstractItemView interface
protected slots:

    /*!
     * \brief verticalScrollbarValueChanged calls QListView method and scrolls the line number column
     * \param value dy of the scroll
     */
    void verticalScrollbarValueChanged(int value) override;

    // QWidget interface
protected:

    /*!
     * \brief mousePressEvent calls the standard mousePressEvent of QListView and then redraws the line number column
     * \param event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*!
     * \brief resizeEvent calls QListView's resize event and resizes the line number column
     * \param e the Resize event
     */
    void resizeEvent(QResizeEvent *e) override;
};

#endif // LOGLISTVIEW_H
