#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class QWidget;
class QPaintEvent;
class QResizeEvent;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    int lineNumberAreaWidth() const;
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    void setLineNumbersVisible(bool visible);
    bool lineNumbersVisible() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

private:
    QWidget *m_lineNumberArea;
    bool m_lineNumbersVisible;
};

#endif // CODEEDITOR_H
