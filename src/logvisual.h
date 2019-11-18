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
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
};

#endif // LOGVISUAL_H
