#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QRadioButton;

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);

    QString findText() const;
    QString replaceText() const;
    bool isCaseSensitive() const;
    bool isBackward() const;

signals:
    void findNextRequested();
    void replaceRequested();
    void replaceAllRequested();

private:
    QLineEdit *m_findLineEdit;
    QLineEdit *m_replaceLineEdit;
    QCheckBox *m_caseCheckBox;
    QRadioButton *m_upRadioButton;
    QRadioButton *m_downRadioButton;
};

#endif // REPLACEDIALOG_H
