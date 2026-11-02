#include "replacedialog.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , m_findLineEdit(new QLineEdit(this))
    , m_replaceLineEdit(new QLineEdit(this))
    , m_caseCheckBox(new QCheckBox(tr("区分大小写"), this))
    , m_upRadioButton(new QRadioButton(tr("向上(&U)"), this))
    , m_downRadioButton(new QRadioButton(tr("向下(&D)"), this))
{
    setWindowTitle(tr("替换"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel *findLabel = new QLabel(tr("查找目标："), this);
    QLabel *replaceLabel = new QLabel(tr("替换为："), this);

    QPushButton *findButton = new QPushButton(tr("查找下一个(&F)"), this);
    QPushButton *replaceButton = new QPushButton(tr("替换(&R)"), this);
    QPushButton *replaceAllButton = new QPushButton(tr("全部替换(&A)"), this);
    QPushButton *cancelButton = new QPushButton(tr("取消"), this);

    findButton->setDefault(true);

    QGroupBox *directionBox = new QGroupBox(tr("方向"), this);
    QHBoxLayout *directionLayout = new QHBoxLayout(directionBox);
    directionLayout->addWidget(m_upRadioButton);
    directionLayout->addWidget(m_downRadioButton);

    m_downRadioButton->setChecked(true);
    m_findLineEdit->setMinimumWidth(220);
    m_replaceLineEdit->setMinimumWidth(220);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(findLabel, 0, 0);
    grid->addWidget(m_findLineEdit, 0, 1);
    grid->addWidget(findButton, 0, 2);
    grid->addWidget(replaceLabel, 1, 0);
    grid->addWidget(m_replaceLineEdit, 1, 1);
    grid->addWidget(replaceButton, 1, 2);
    grid->addWidget(m_caseCheckBox, 2, 0);
    grid->addWidget(directionBox, 2, 1);
    grid->addWidget(replaceAllButton, 2, 2);
    grid->addWidget(cancelButton, 3, 2);
    grid->setColumnStretch(1, 1);

    setLayout(grid);

    connect(findButton, &QPushButton::clicked, this, &ReplaceDialog::findNextRequested);
    connect(replaceButton, &QPushButton::clicked, this, &ReplaceDialog::replaceRequested);
    connect(replaceAllButton, &QPushButton::clicked, this, &ReplaceDialog::replaceAllRequested);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_findLineEdit, &QLineEdit::returnPressed, this, &ReplaceDialog::findNextRequested);
    connect(m_replaceLineEdit, &QLineEdit::returnPressed, this, &ReplaceDialog::replaceRequested);
}

QString ReplaceDialog::findText() const
{
    return m_findLineEdit->text();
}

QString ReplaceDialog::replaceText() const
{
    return m_replaceLineEdit->text();
}

bool ReplaceDialog::isCaseSensitive() const
{
    return m_caseCheckBox->isChecked();
}

bool ReplaceDialog::isBackward() const
{
    return m_upRadioButton->isChecked();
}
