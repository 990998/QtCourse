#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QCloseEvent;
class QLabel;
class QTimer;
class FindDialog;
class ReplaceDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();
    void exitApplication();

    void showFindDialog();
    void findNext();
    void showReplaceDialog();
    void replaceNext();
    void replaceAll();

    void chooseFont();
    void chooseTextColor();
    void chooseTextBackgroundColor();
    void chooseEditorBackgroundColor();
    void setWordWrap(bool enabled);
    void setLineNumbersVisible(bool visible);

    void toggleToolBar(bool visible);
    void toggleStatusBar(bool visible);
    void toggleAutoSave(bool enabled);
    void autoSave();

    void updateStatusBar();
    void updateEditActions();

    void showAbout();

private:
    bool maybeSave();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool findTextInEditor(const QString &text, bool caseSensitive,
                          bool backward, bool showNotFound);
    void createStatusBar();
    void setActionIcons();
    void createConnections();

    Ui::MainWindow *ui;
    QString m_currentFile;
    FindDialog *m_findDialog;
    ReplaceDialog *m_replaceDialog;
    QTimer *m_autoSaveTimer;

    QLabel *m_lengthLabel;
    QLabel *m_linesLabel;
    QLabel *m_positionLabel;
    QLabel *m_authorLabel;
};

#endif // MAINWINDOW_H
