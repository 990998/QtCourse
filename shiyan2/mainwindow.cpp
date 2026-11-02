#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeeditor.h"
#include "finddialog.h"
#include "replacedialog.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QColorDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QStatusBar>
#include <QStyle>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>
#include <QTimer>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_findDialog(nullptr)
    , m_replaceDialog(nullptr)
    , m_autoSaveTimer(new QTimer(this))
    , m_lengthLabel(nullptr)
    , m_linesLabel(nullptr)
    , m_positionLabel(nullptr)
    , m_authorLabel(nullptr)
{
    ui->setupUi(this);

    setWindowIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui->toolBar->setIconSize(QSize(24, 24));
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_autoSaveTimer->setInterval(60 * 1000);

    createStatusBar();
    setActionIcons();
    setCurrentFile(QString());
    createConnections();
    updateStatusBar();
    updateEditActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (!maybeSave()) {
        return;
    }

    ui->editor->clear();
    setCurrentFile(QString());
    statusBar()->showMessage(tr("已新建文件"), 2000);
}

void MainWindow::openFile()
{
    if (!maybeSave()) {
        return;
    }

    const QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开文件"),
        m_currentFile.isEmpty() ? QString() : QFileInfo(m_currentFile).absolutePath(),
        tr("文本文件 (*.txt *.md *.cpp *.h *.log);;所有文件 (*.*)"));

    if (fileName.isEmpty()) {
        return;
    }

    loadFile(fileName);
}

bool MainWindow::save()
{
    if (m_currentFile.isEmpty()) {
        return saveAs();
    }
    return saveFile(m_currentFile);
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("另存为"),
        m_currentFile.isEmpty() ? QString() : m_currentFile,
        tr("文本文件 (*.txt);;所有文件 (*.*)"));

    if (fileName.isEmpty()) {
        return false;
    }

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName += QLatin1String(".txt");
    }

    return saveFile(fileName);
}

void MainWindow::exitApplication()
{
    close();
}

void MainWindow::showFindDialog()
{
    if (!m_findDialog) {
        m_findDialog = new FindDialog(this);
        connect(m_findDialog, &FindDialog::findNextRequested,
                this, &MainWindow::findNext);
    }

    m_findDialog->show();
    m_findDialog->raise();
    m_findDialog->activateWindow();
}

void MainWindow::findNext()
{
    QString text;
    bool caseSensitive = false;
    bool backward = false;

    if (sender() == m_replaceDialog && m_replaceDialog) {
        text = m_replaceDialog->findText();
        caseSensitive = m_replaceDialog->isCaseSensitive();
        backward = m_replaceDialog->isBackward();
    } else if (m_findDialog) {
        text = m_findDialog->findText();
        caseSensitive = m_findDialog->isCaseSensitive();
        backward = m_findDialog->isBackward();
    } else {
        return;
    }

    if (text.isEmpty()) {
        statusBar()->showMessage(tr("请输入要查找的内容"), 3000);
        return;
    }

    findTextInEditor(text, caseSensitive, backward, true);
}

void MainWindow::showReplaceDialog()
{
    if (!m_replaceDialog) {
        m_replaceDialog = new ReplaceDialog(this);
        connect(m_replaceDialog, &ReplaceDialog::findNextRequested,
                this, &MainWindow::findNext);
        connect(m_replaceDialog, &ReplaceDialog::replaceRequested,
                this, &MainWindow::replaceNext);
        connect(m_replaceDialog, &ReplaceDialog::replaceAllRequested,
                this, &MainWindow::replaceAll);
    }

    m_replaceDialog->show();
    m_replaceDialog->raise();
    m_replaceDialog->activateWindow();
}

void MainWindow::replaceNext()
{
    if (!m_replaceDialog) {
        return;
    }

    const QString findText = m_replaceDialog->findText();
    if (findText.isEmpty()) {
        QMessageBox::information(this, tr("替换"), tr("请输入要查找的内容。"));
        return;
    }

    const bool caseSensitive = m_replaceDialog->isCaseSensitive();
    const bool backward = m_replaceDialog->isBackward();
    const Qt::CaseSensitivity cs = caseSensitive
            ? Qt::CaseSensitive : Qt::CaseInsensitive;

    QTextCursor cursor = ui->editor->textCursor();
    bool selectionMatches = cursor.hasSelection()
            && cursor.selectedText().compare(findText, cs) == 0;

    if (!selectionMatches) {
        if (!findTextInEditor(findText, caseSensitive, backward, true)) {
            return;
        }
        cursor = ui->editor->textCursor();
    }

    const int startPosition = cursor.selectionStart();
    cursor.insertText(m_replaceDialog->replaceText());
    if (backward) {
        cursor.setPosition(startPosition);
    }
    ui->editor->setTextCursor(cursor);

    findTextInEditor(findText, caseSensitive, backward, false);
}

void MainWindow::replaceAll()
{
    if (!m_replaceDialog) {
        return;
    }

    const QString findText = m_replaceDialog->findText();
    if (findText.isEmpty()) {
        QMessageBox::information(this, tr("替换"), tr("请输入要查找的内容。"));
        return;
    }

    const QString replacement = m_replaceDialog->replaceText();
    const bool caseSensitive = m_replaceDialog->isCaseSensitive();
    const bool backward = m_replaceDialog->isBackward();

    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (backward) {
        flags |= QTextDocument::FindBackward;
    }

    QTextCursor editCursor(ui->editor->document());
    editCursor.beginEditBlock();
    editCursor.movePosition(backward ? QTextCursor::End : QTextCursor::Start);
    ui->editor->setTextCursor(editCursor);

    int count = 0;
    while (ui->editor->find(findText, flags)) {
        QTextCursor cursor = ui->editor->textCursor();
        const int startPosition = cursor.selectionStart();
        cursor.insertText(replacement);
        if (backward) {
            cursor.setPosition(startPosition);
        }
        ui->editor->setTextCursor(cursor);
        ++count;
    }
    editCursor.endEditBlock();

    if (count > 0) {
        QMessageBox::information(this, tr("替换"),
                                 tr("共替换 %1 处。").arg(count));
    } else {
        QMessageBox::information(this, tr("替换"),
                                 tr("未找到“%1”。").arg(findText));
    }
}

void MainWindow::chooseFont()
{
    bool ok = false;
    const QFont font = QFontDialog::getFont(&ok, ui->editor->font(), this, tr("选择字体"));
    if (ok) {
        ui->editor->setFont(font);
        ui->editor->setLineNumbersVisible(ui->actionShowLineNumber->isChecked());
    }
}

void MainWindow::chooseTextColor()
{
    QColor initial = ui->editor->currentCharFormat().foreground().color();
    if (!initial.isValid()) {
        initial = ui->editor->palette().color(QPalette::Text);
    }

    const QColor color = QColorDialog::getColor(initial, this, tr("字体颜色"));
    if (!color.isValid()) {
        return;
    }

    QTextCharFormat format;
    format.setForeground(color);
    ui->editor->mergeCurrentCharFormat(format);
}

void MainWindow::chooseTextBackgroundColor()
{
    QColor initial = ui->editor->currentCharFormat().background().color();
    if (!initial.isValid()) {
        initial = ui->editor->palette().color(QPalette::Base);
    }

    const QColor color = QColorDialog::getColor(initial, this, tr("字体背景色"));
    if (!color.isValid()) {
        return;
    }

    QTextCharFormat format;
    format.setBackground(color);
    ui->editor->mergeCurrentCharFormat(format);
}

void MainWindow::chooseEditorBackgroundColor()
{
    const QColor color = QColorDialog::getColor(
        ui->editor->palette().color(QPalette::Base), this, tr("编辑器背景色"));

    if (!color.isValid()) {
        return;
    }

    QPalette palette = ui->editor->palette();
    palette.setColor(QPalette::Base, color);
    ui->editor->setPalette(palette);
}

void MainWindow::setWordWrap(bool enabled)
{
    ui->editor->setLineWrapMode(enabled
                                ? QPlainTextEdit::WidgetWidth
                                : QPlainTextEdit::NoWrap);
}

void MainWindow::setLineNumbersVisible(bool visible)
{
    ui->editor->setLineNumbersVisible(visible);
}

void MainWindow::toggleToolBar(bool visible)
{
    ui->toolBar->setVisible(visible);
}

void MainWindow::toggleStatusBar(bool visible)
{
    ui->statusbar->setVisible(visible);
}

void MainWindow::toggleAutoSave(bool enabled)
{
    if (enabled) {
        m_autoSaveTimer->start();
        statusBar()->showMessage(tr("已开启定时自动保存（每分钟）"), 3000);
    } else {
        m_autoSaveTimer->stop();
        statusBar()->showMessage(tr("已关闭定时自动保存"), 3000);
    }
}

void MainWindow::autoSave()
{
    if (m_currentFile.isEmpty() || !ui->editor->document()->isModified()) {
        return;
    }

    if (saveFile(m_currentFile)) {
        statusBar()->showMessage(tr("已自动保存"), 2000);
    }
}

void MainWindow::updateStatusBar()
{
    const QString text = ui->editor->toPlainText();
    const QTextCursor cursor = ui->editor->textCursor();

    m_lengthLabel->setText(tr("length: %1").arg(text.length()));
    m_linesLabel->setText(tr("lines: %1").arg(ui->editor->blockCount()));
    m_positionLabel->setText(tr("Ln: %1  Col: %2")
                             .arg(cursor.blockNumber() + 1)
                             .arg(cursor.positionInBlock() + 1));
}

void MainWindow::updateEditActions()
{
    const QTextCursor cursor = ui->editor->textCursor();
    const bool hasSelection = cursor.hasSelection();

    ui->actionUndo->setEnabled(ui->editor->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(ui->editor->document()->isRedoAvailable());
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);

    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    ui->actionPaste->setEnabled(mimeData && mimeData->hasText());
}

void MainWindow::showAbout()
{
    QMessageBox::about(
        this,
        tr("关于"),
        tr("<h3>简单文本编辑器</h3>"
           "<p>版本：V1.0 build 2026</p>"
           "<p>基于 Qt %1 开发</p>"
           "<p>功能：文件读写、撤销重做、剪贴板、查找替换、"
           "字体与颜色设置、行号显示、自动换行、定时自动保存。</p>")
            .arg(QString::fromLatin1(QT_VERSION_STR)));
}

bool MainWindow::maybeSave()
{
    if (!ui->editor->document()->isModified()) {
        return true;
    }

    const QMessageBox::StandardButton ret = QMessageBox::warning(
        this,
        tr("编辑器"),
        tr("文档已被修改，是否保存修改？"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (ret == QMessageBox::Save) {
        return save();
    }
    if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("编辑器"),
            tr("无法打开文件 %1：\n%2")
                .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    const QString content = in.readAll();
    file.close();

    ui->editor->setPlainText(content);
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("已打开 %1").arg(QDir::toNativeSeparators(fileName)),
                             3000);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("编辑器"),
            tr("无法保存文件 %1：\n%2")
                .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << ui->editor->toPlainText();

    if (out.status() != QTextStream::Ok) {
        QMessageBox::warning(
            this,
            tr("编辑器"),
            tr("写入文件 %1 时发生错误。").arg(QDir::toNativeSeparators(fileName)));
        file.close();
        return false;
    }

    file.close();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("已保存 %1").arg(QDir::toNativeSeparators(fileName)),
                             3000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_currentFile = fileName;
    ui->editor->document()->setModified(false);
    setWindowModified(false);

    const QString shownName = m_currentFile.isEmpty()
            ? tr("新建文本文件")
            : QFileInfo(m_currentFile).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName, tr("编辑器")));
}

bool MainWindow::findTextInEditor(const QString &text, bool caseSensitive,
                                  bool backward, bool showNotFound)
{
    if (text.isEmpty()) {
        return false;
    }

    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (backward) {
        flags |= QTextDocument::FindBackward;
    }

    bool found = ui->editor->find(text, flags);

    if (!found) {
        QTextCursor cursor = ui->editor->textCursor();
        cursor.movePosition(backward ? QTextCursor::End : QTextCursor::Start);
        ui->editor->setTextCursor(cursor);
        found = ui->editor->find(text, flags);
    }

    if (!found && showNotFound) {
        QMessageBox::information(this, tr("查找"),
                                 tr("未找到“%1”。").arg(text));
    }

    return found;
}

void MainWindow::createStatusBar()
{
    m_lengthLabel = new QLabel(tr("length: 0"), this);
    m_linesLabel = new QLabel(tr("lines: 1"), this);
    m_positionLabel = new QLabel(tr("Ln: 1  Col: 1"), this);
    m_authorLabel = new QLabel(tr("Qt 文本编辑器"), this);

    m_lengthLabel->setMinimumWidth(80);
    m_linesLabel->setMinimumWidth(70);
    m_positionLabel->setMinimumWidth(110);

    statusBar()->addPermanentWidget(m_lengthLabel);
    statusBar()->addPermanentWidget(m_linesLabel);
    statusBar()->addPermanentWidget(m_positionLabel);
    statusBar()->addPermanentWidget(m_authorLabel);
    statusBar()->showMessage(tr("就绪"));
}

void MainWindow::setActionIcons()
{
    ui->actionNew->setIcon(QIcon(QStringLiteral(":/icons/new.svg")));
    ui->actionOpen->setIcon(QIcon(QStringLiteral(":/icons/open.svg")));
    ui->actionSave->setIcon(QIcon(QStringLiteral(":/icons/save.svg")));
    ui->actionSaveAs->setIcon(QIcon(QStringLiteral(":/icons/save_as.svg")));
    ui->actionExit->setIcon(QIcon(QStringLiteral(":/icons/exit.svg")));

    ui->actionUndo->setIcon(QIcon(QStringLiteral(":/icons/undo.svg")));
    ui->actionRedo->setIcon(QIcon(QStringLiteral(":/icons/redo.svg")));
    ui->actionCut->setIcon(QIcon(QStringLiteral(":/icons/cut.svg")));
    ui->actionCopy->setIcon(QIcon(QStringLiteral(":/icons/copy.svg")));
    ui->actionPaste->setIcon(QIcon(QStringLiteral(":/icons/paste.svg")));
    ui->actionFind->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    ui->actionReplace->setIcon(QIcon(QStringLiteral(":/icons/replace.svg")));
    ui->actionSelectAll->setIcon(QIcon(QStringLiteral(":/icons/select_all.svg")));

    ui->actionWordWrap->setIcon(QIcon(QStringLiteral(":/icons/wrap.svg")));
    ui->actionShowLineNumber->setIcon(QIcon(QStringLiteral(":/icons/line_number.svg")));
    ui->actionFont->setIcon(QIcon(QStringLiteral(":/icons/font.svg")));
    ui->actionTextColor->setIcon(QIcon(QStringLiteral(":/icons/text_color.svg")));
    ui->actionTextBackgroundColor->setIcon(QIcon(QStringLiteral(":/icons/text_background.svg")));
    ui->actionEditorBackgroundColor->setIcon(QIcon(QStringLiteral(":/icons/editor_background.svg")));

    ui->actionShowToolBar->setIcon(QIcon(QStringLiteral(":/icons/toolbar.svg")));
    ui->actionShowStatusBar->setIcon(QIcon(QStringLiteral(":/icons/statusbar.svg")));
    ui->actionAbout->setIcon(QIcon(QStringLiteral(":/icons/about.svg")));
}

void MainWindow::createConnections()
{
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(ui->actionAutoSave, &QAction::toggled, this, &MainWindow::toggleAutoSave);
    connect(m_autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);

    connect(ui->actionUndo, &QAction::triggered, ui->editor, &QPlainTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->editor, &QPlainTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered, ui->editor, &QPlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->editor, &QPlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->editor, &QPlainTextEdit::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->editor, &QPlainTextEdit::selectAll);

    connect(ui->actionFind, &QAction::triggered, this, &MainWindow::showFindDialog);
    connect(ui->actionReplace, &QAction::triggered, this, &MainWindow::showReplaceDialog);

    connect(ui->actionWordWrap, &QAction::toggled, this, &MainWindow::setWordWrap);
    connect(ui->actionShowLineNumber, &QAction::toggled,
            this, &MainWindow::setLineNumbersVisible);
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::chooseFont);
    connect(ui->actionTextColor, &QAction::triggered,
            this, &MainWindow::chooseTextColor);
    connect(ui->actionTextBackgroundColor, &QAction::triggered,
            this, &MainWindow::chooseTextBackgroundColor);
    connect(ui->actionEditorBackgroundColor, &QAction::triggered,
            this, &MainWindow::chooseEditorBackgroundColor);

    connect(ui->actionShowToolBar, &QAction::toggled, this, &MainWindow::toggleToolBar);
    connect(ui->actionShowStatusBar, &QAction::toggled, this, &MainWindow::toggleStatusBar);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

    connect(ui->editor, &QPlainTextEdit::textChanged,
            this, &MainWindow::updateStatusBar);
    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateStatusBar);
    connect(ui->editor, &QPlainTextEdit::copyAvailable, this,
            [this](bool available) {
                ui->actionCopy->setEnabled(available);
                ui->actionCut->setEnabled(available);
            });
    connect(ui->editor->document(), &QTextDocument::undoAvailable,
            ui->actionUndo, &QAction::setEnabled);
    connect(ui->editor->document(), &QTextDocument::redoAvailable,
            ui->actionRedo, &QAction::setEnabled);
    connect(ui->editor->document(), &QTextDocument::modificationChanged,
            this, [this](bool changed) {
                setWindowModified(changed);
            });
    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::updateEditActions);
}
