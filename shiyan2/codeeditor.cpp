#include "codeeditor.h"

#include <QFontDatabase>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTextBlock>

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodeEditor *editor)
        : QWidget(editor)
        , m_editor(editor)
    {
    }

    QSize sizeHint() const override
    {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        m_editor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *m_editor;
};

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_lineNumberArea(new LineNumberArea(this))
    , m_lineNumbersVisible(true)
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(11);
    setFont(font);

    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    setTabStopWidth(fontMetrics().horizontalAdvance(QLatin1Char(' ')) * 4);

    connect(this, &QPlainTextEdit::blockCountChanged,
            this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest,
            this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    const int space = 12 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(245, 245, 245));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(128, 128, 128));
            painter.drawText(0, top,
                             m_lineNumberArea->width() - 8,
                             fontMetrics().height(),
                             Qt::AlignRight,
                             number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::setLineNumbersVisible(bool visible)
{
    m_lineNumbersVisible = visible;
    m_lineNumberArea->setVisible(visible);
    updateLineNumberAreaWidth(blockCount());
    updateLineNumberArea(viewport()->rect(), 0);
}

bool CodeEditor::lineNumbersVisible() const
{
    return m_lineNumbersVisible;
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    const QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                        m_lineNumbersVisible ? lineNumberAreaWidth() : 0,
                                        cr.height()));
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
    const int width = m_lineNumbersVisible ? lineNumberAreaWidth() : 0;
    setViewportMargins(width, 0, 0, 0);

    const QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), width, cr.height()));
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::highlightCurrentLine()
{
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(255, 255, 200));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}
