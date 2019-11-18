#ifndef LOGVISUAL_H
#define LOGVISUAL_H

#include <QPlainTextEdit>
#include <QWidget>

class LogVisual : public QPlainTextEdit
{
    Q_OBJECT

public:
    LogVisual(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    /*!
     * \brief lineNumberAreaWidth calculates the width taken by the number area
     * \return the width taken by the number area
     */
    int lineNumberAreaWidth();



protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    /*!
     * \brief pushLine pushes a line to the Form
     * \param line the line to insert
     */
    void pushLine(QString line);

signals:
    void lineAdded(QString line);

private:
    QWidget *lineNumberArea;
};

#endif // LOGVISUAL_H
