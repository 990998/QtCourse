#include "finddialog.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , m_findLineEdit(new QLineEdit(this))
    , m_caseCheckBox(new QCheckBox(tr("区分大小写"), this))
    , m_upRadioButton(new QRadioButton(tr("向上(&U)"), this))
    , m_downRadioButton(new QRadioButton(tr("向下(&D)"), this))
{
    setWindowTitle(tr("查找"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel *findLabel = new QLabel(tr("查找目标："), this);
    QPushButton *findButton = new QPushButton(tr("查找下一个(&F)"), this);
    QPushButton *cancelButton = new QPushButton(tr("取消"), this);

    findButton->setDefault(true);

    QGroupBox *directionBox = new QGroupBox(tr("方向"), this);
    QHBoxLayout *directionLayout = new QHBoxLayout(directionBox);
    directionLayout->addWidget(m_upRadioButton);
    directionLayout->addWidget(m_downRadioButton);

    m_downRadioButton->setChecked(true);
    m_findLineEdit->setMinimumWidth(220);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(findLabel, 0, 0);
    grid->addWidget(m_findLineEdit, 0, 1);
    grid->addWidget(findButton, 0, 2);
    grid->addWidget(m_caseCheckBox, 1, 0);
    grid->addWidget(directionBox, 1, 1);
    grid->addWidget(cancelButton, 1, 2);
    grid->setColumnStretch(1, 1);

    setLayout(grid);

    connect(findButton, &QPushButton::clicked, this, &FindDialog::findNextRequested);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_findLineEdit, &QLineEdit::returnPressed, this, &FindDialog::findNextRequested);
}

QString FindDialog::findText() const
{
    return m_findLineEdit->text();
}

bool FindDialog::isCaseSensitive() const
{
    return m_caseCheckBox->isChecked();
}

bool FindDialog::isBackward() const
{
    return m_upRadioButton->isChecked();
}
