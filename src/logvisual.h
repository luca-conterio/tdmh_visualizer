#ifndef LOGVISUAL_H
#define LOGVISUAL_H

#include <QPlainTextEdit>
#include <QWidget>

/*!
 * \brief The LogVisual class is a Widget that contains the text and adds line numbers
 */
class LogVisual : public QPlainTextEdit
{
    Q_OBJECT

public:
    /*!
     * \brief LogVisual builds a text edit that contains the log
     * \param parent the parent widget
     */
    LogVisual(QWidget *parent = nullptr);

    /*!
     * \brief lineNumberAreaPaintEvent draws the digits on the number column of the editor
     * \param event the paint event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    /*!
     * \brief lineNumberAreaWidth calculates the width taken by the number area
     * \return the width taken by the number area
     */
    int lineNumberAreaWidth();



protected:
    /*!
     * \brief resizeEvent overrides standard resize event to account for line numers
     * \param event the resize event
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    /*!
     * \brief pushLine pushes a line to the Form
     * \param line the line to insert
     */
    void pushLine(const QString& line);

signals:
    /*!
     * \brief lineAdded signal to be raised to insert a new line
     * \param line the line to add
     */
    void lineAdded(QString line);

private:
    QWidget *lineNumberArea;
};

#endif // LOGVISUAL_H
