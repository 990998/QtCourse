#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QRadioButton;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);

    QString findText() const;
    bool isCaseSensitive() const;
    bool isBackward() const;

signals:
    void findNextRequested();

private:
    QLineEdit *m_findLineEdit;
    QCheckBox *m_caseCheckBox;
    QRadioButton *m_upRadioButton;
    QRadioButton *m_downRadioButton;
};

#endif // FINDDIALOG_H
