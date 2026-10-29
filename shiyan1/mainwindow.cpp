#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QPushButton>
#include <QSizePolicy>

#include <cmath>

namespace {
// 最多允许输入的有效数字位数，避免无限输入导致溢出
const int kMaxDigits = 16;

// 统计字符串中的数字字符个数（忽略符号与小数点）
int digitCount(const QString &text)
{
    int count = 0;
    for (const QChar &c : text) {
        if (c.isDigit())
            ++count;
    }
    return count;
}

// 将内部运算符转换为界面显示符号
QString opSymbol(const QString &op)
{
    if (op == "+") return QStringLiteral("+");
    if (op == "-") return QStringLiteral("−");
    if (op == "*") return QStringLiteral("×");
    if (op == "/") return QStringLiteral("÷");
    return op;
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("计算器"));
    setFixedSize(380, 560);

    m_current = QStringLiteral("0");

    // 让窗口（而不是按钮 / 显示框）接收键盘事件，从而统一由 keyPressEvent 处理
    const QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
    ui->displayEdit->setFocusPolicy(Qt::NoFocus);

    connectButtons();
    applyStyleSheet();
    refreshDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 统一把每个按钮的点击事件连接到与键盘完全相同的动作方法
void MainWindow::connectButtons()
{
    connect(ui->btn0, &QPushButton::clicked, this, [this]() { inputDigit(0); });
    connect(ui->btn1, &QPushButton::clicked, this, [this]() { inputDigit(1); });
    connect(ui->btn2, &QPushButton::clicked, this, [this]() { inputDigit(2); });
    connect(ui->btn3, &QPushButton::clicked, this, [this]() { inputDigit(3); });
    connect(ui->btn4, &QPushButton::clicked, this, [this]() { inputDigit(4); });
    connect(ui->btn5, &QPushButton::clicked, this, [this]() { inputDigit(5); });
    connect(ui->btn6, &QPushButton::clicked, this, [this]() { inputDigit(6); });
    connect(ui->btn7, &QPushButton::clicked, this, [this]() { inputDigit(7); });
    connect(ui->btn8, &QPushButton::clicked, this, [this]() { inputDigit(8); });
    connect(ui->btn9, &QPushButton::clicked, this, [this]() { inputDigit(9); });

    connect(ui->btnDecimal, &QPushButton::clicked, this, [this]() { inputDecimal(); });
    connect(ui->btnAdd, &QPushButton::clicked, this, [this]() { inputOperator(QStringLiteral("+")); });
    connect(ui->btnSub, &QPushButton::clicked, this, [this]() { inputOperator(QStringLiteral("-")); });
    connect(ui->btnMul, &QPushButton::clicked, this, [this]() { inputOperator(QStringLiteral("*")); });
    connect(ui->btnDiv, &QPushButton::clicked, this, [this]() { inputOperator(QStringLiteral("/")); });
    connect(ui->btnEqual, &QPushButton::clicked, this, [this]() { calculateResult(); });

    connect(ui->btnClear, &QPushButton::clicked, this, [this]() { clearAll(); });
    connect(ui->btnCE, &QPushButton::clicked, this, [this]() { clearEntry(); });
    connect(ui->btnBackspace, &QPushButton::clicked, this, [this]() { backspace(); });
    connect(ui->btnSign, &QPushButton::clicked, this, [this]() { toggleSign(); });

    connect(ui->btnPercent, &QPushButton::clicked, this, [this]() { inputPercent(); });
    connect(ui->btnReciprocal, &QPushButton::clicked, this, [this]() { applyReciprocal(); });
    connect(ui->btnSquare, &QPushButton::clicked, this, [this]() { applySquare(); });
    connect(ui->btnSqrt, &QPushButton::clicked, this, [this]() { applySqrt(); });
}

// 键盘事件：把按键映射到与鼠标按钮相同的动作方法
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const int key = event->key();

    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        inputDigit(key - Qt::Key_0);
        return;
    }

    switch (key) {
    case Qt::Key_Period:
    case Qt::Key_Comma:
        inputDecimal();
        return;
    case Qt::Key_Plus:
        inputOperator(QStringLiteral("+"));
        return;
    case Qt::Key_Minus:
        inputOperator(QStringLiteral("-"));
        return;
    case Qt::Key_Asterisk:
        inputOperator(QStringLiteral("*"));
        return;
    case Qt::Key_Slash:
        inputOperator(QStringLiteral("/"));
        return;
    case Qt::Key_Equal:
    case Qt::Key_Return:
    case Qt::Key_Enter:
        calculateResult();
        return;
    case Qt::Key_Backspace:
        backspace();
        return;
    case Qt::Key_Delete:
        clearEntry();
        return;
    case Qt::Key_Escape:
        clearAll();
        return;
    case Qt::Key_Percent:
        inputPercent();
        return;
    default:
        break;
    }

    QMainWindow::keyPressEvent(event);
}

// ---------------- 输入动作 ----------------

void MainWindow::inputDigit(int digit)
{
    if (m_error)
        clearAll();

    const QString d = QString::number(digit);

    if (m_freshInput) {
        // 开始输入新的操作数
        m_current = d;
        m_freshInput = false;
    } else if (m_current == QStringLiteral("0")) {
        m_current = d;                       // 去掉无意义的前导 0
    } else if (m_current == QStringLiteral("-0")) {
        m_current = QStringLiteral("-") + d;
    } else if (digitCount(m_current) < kMaxDigits) {
        m_current += d;
    }

    refreshDisplay();
}

void MainWindow::inputDecimal()
{
    if (m_error)
        clearAll();

    if (m_freshInput) {
        m_current = QStringLiteral("0.");
        m_freshInput = false;
    } else if (!m_current.contains(QLatin1Char('.'))) {
        // 关键：同一操作数内只允许一个小数点，重复输入被忽略
        m_current += QLatin1Char('.');
    }

    refreshDisplay();
}

void MainWindow::inputOperator(const QString &op)
{
    if (m_error)
        return;

    if (!m_pendingOp.isEmpty() && m_freshInput) {
        // 连续按运算符：只替换运算符，例如 5 + 再按 × -> 5 ×
        m_pendingOp = op;
        refreshDisplay();
        return;
    }

    if (!m_pendingOp.isEmpty()) {
        // 连续运算：先把前面的式子算出来作为新的左操作数
        double result = 0.0;
        if (!applyOperation(m_pendingOp, m_accumulator, m_current.toDouble(), result)) {
            setError(QStringLiteral("除数不能为 0"));
            return;
        }
        m_accumulator = result;
        m_leftText = formatNumber(result);
        m_current = m_leftText;
    } else {
        // 第一次按运算符，记录第一个操作数
        m_accumulator = m_current.toDouble();
        m_leftText = m_current;
    }

    m_pendingOp = op;
    m_freshInput = true; // 等待输入第二个操作数
    refreshDisplay();
}

void MainWindow::calculateResult()
{
    if (m_error)
        return;

    if (m_pendingOp.isEmpty()) {
        // 没有待处理运算符，仅规范化当前显示
        m_current = formatNumber(m_current.toDouble());
        m_freshInput = true;
        refreshDisplay();
        return;
    }

    const QString expression = m_leftText + QLatin1Char(' ')
            + opSymbol(m_pendingOp) + QLatin1Char(' ') + m_current;

    double result = 0.0;
    if (!applyOperation(m_pendingOp, m_accumulator, m_current.toDouble(), result)) {
        setError(QStringLiteral("除数不能为 0"));
        return;
    }

    m_current = formatNumber(result);
    m_accumulator = result;
    m_pendingOp.clear();
    m_leftText.clear();
    m_freshInput = true;

    refreshDisplay();
    // 在副显示区保留 "操作数 运算符 操作数 =" 便于查看完整算式
    ui->resultLabel->setText(expression + QStringLiteral(" ="));
}

void MainWindow::backspace()
{
    if (m_error) {
        clearAll();
        return;
    }
    if (m_freshInput)
        return; // 刚按完运算符或显示结果时不退格

    m_current.chop(1);
    if (m_current.isEmpty() || m_current == QStringLiteral("-")) {
        m_current = QStringLiteral("0");
        m_freshInput = true;
    }
    refreshDisplay();
}

void MainWindow::clearAll()
{
    m_accumulator = 0.0;
    m_leftText.clear();
    m_current = QStringLiteral("0");
    m_pendingOp.clear();
    m_freshInput = true;
    m_error = false;
    m_errorMessage.clear();

    ui->resultLabel->clear();
    refreshDisplay();
}

void MainWindow::clearEntry()
{
    if (m_error) {
        clearAll();
        return;
    }
    // 只清除当前操作数，保留前面已输入的运算符
    m_current = QStringLiteral("0");
    m_freshInput = true;
    refreshDisplay();
}

void MainWindow::toggleSign()
{
    if (m_error || m_current == QStringLiteral("0") || m_current.isEmpty())
        return;

    if (m_current.startsWith(QLatin1Char('-')))
        m_current.remove(0, 1);
    else
        m_current.prepend(QLatin1Char('-'));

    refreshDisplay();
}

void MainWindow::inputPercent()
{
    if (m_error)
        return;

    double value = m_current.toDouble();
    // 加减法：对左操作数取百分比；乘除法：当前数直接除以 100
    if (m_pendingOp == QStringLiteral("+") || m_pendingOp == QStringLiteral("-"))
        value = m_accumulator * value / 100.0;
    else
        value = value / 100.0;

    m_current = formatNumber(value);
    m_freshInput = true;
    refreshDisplay();
}

void MainWindow::applyReciprocal()
{
    if (m_error)
        return;

    const double value = m_current.toDouble();
    if (qFuzzyIsNull(value)) {
        setError(QStringLiteral("除数不能为 0"));
        return;
    }
    m_current = formatNumber(1.0 / value);
    m_freshInput = true;
    refreshDisplay();
}

void MainWindow::applySquare()
{
    if (m_error)
        return;

    const double value = m_current.toDouble();
    m_current = formatNumber(value * value);
    m_freshInput = true;
    refreshDisplay();
}

void MainWindow::applySqrt()
{
    if (m_error)
        return;

    const double value = m_current.toDouble();
    if (value < 0.0) {
        setError(QStringLiteral("无效输入"));
        return;
    }
    m_current = formatNumber(std::sqrt(value));
    m_freshInput = true;
    refreshDisplay();
}

// ---------------- 内部辅助 ----------------

// 纯计算函数：只做运算并通过返回值报告是否成功，不产生任何界面副作用，
// 因此实时预览和正式求值都可以安全调用。
bool MainWindow::applyOperation(const QString &op, double lhs, double rhs, double &result) const
{
    if (op == QStringLiteral("+")) {
        result = lhs + rhs;
    } else if (op == QStringLiteral("-")) {
        result = lhs - rhs;
    } else if (op == QStringLiteral("*")) {
        result = lhs * rhs;
    } else if (op == QStringLiteral("/")) {
        if (rhs == 0.0)
            return false; // 除数为 0，由调用方负责提示
        result = lhs / rhs;
    } else {
        return false;
    }
    return true;
}

QString MainWindow::formatNumber(double value) const
{
    if (qIsNaN(value) || qIsInf(value))
        return QStringLiteral("错误");

    // 用 15 位有效数字格式化，可消除浮点误差（如 0.1+0.2）
    return QString::number(value, 'g', 15);
}

void MainWindow::refreshDisplay()
{
    if (m_error) {
        ui->displayEdit->setText(m_errorMessage);
        ui->resultLabel->clear();
        return;
    }

    if (m_pendingOp.isEmpty()) {
        ui->displayEdit->setText(m_current);
        ui->resultLabel->clear();
        return;
    }

    // 正在输入一个双目表达式
    QString text = m_leftText + QLatin1Char(' ') + opSymbol(m_pendingOp);
    if (!m_freshInput) {
        text += QLatin1Char(' ') + m_current;

        // 实时预览结果
        double preview = 0.0;
        if (applyOperation(m_pendingOp, m_accumulator, m_current.toDouble(), preview))
            ui->resultLabel->setText(QStringLiteral("= ") + formatNumber(preview));
        else
            ui->resultLabel->clear();
    } else {
        ui->resultLabel->clear();
    }
    ui->displayEdit->setText(text);
}

void MainWindow::setError(const QString &message)
{
    m_error = true;
    m_errorMessage = message;
    refreshDisplay();
}

void MainWindow::applyStyleSheet()
{
    setStyleSheet(QStringLiteral(R"(
        QMainWindow { background: #f3f3f3; }
        QLineEdit#displayEdit {
            background: #2b2b2b;
            color: #ffffff;
            border: none;
            border-radius: 10px;
            padding: 8px 16px;
        }
        QLabel#resultLabel { color: #6b6b6b; }
        QPushButton {
            background: #ffffff;
            border: 1px solid #d9d9d9;
            border-radius: 10px;
            font-size: 17px;
            min-height: 48px;
        }
        QPushButton:hover { background: #eef6ff; }
        QPushButton:pressed { background: #d6e9ff; }
        QPushButton#btnAdd, QPushButton#btnSub,
        QPushButton#btnMul, QPushButton#btnDiv {
            font-size: 22px;
            color: #1a5fb4;
        }
        QPushButton#btnClear, QPushButton#btnCE { color: #c0392b; }
        QPushButton#btnEqual {
            background: #0078d4;
            border: none;
            color: #ffffff;
            font-size: 22px;
        }
        QPushButton#btnEqual:hover { background: #1a86d9; }
        QPushButton#btnEqual:pressed { background: #006cbe; }
    )"));
}
