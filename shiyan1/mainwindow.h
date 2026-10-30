#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QKeyEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 键盘事件：与鼠标按钮复用同一套输入动作
    void keyPressEvent(QKeyEvent *event) override;

private:
    // ---- 输入动作（鼠标点击与键盘事件都会调用这些方法）----
    void inputDigit(int digit);            // 数字 0-9
    void inputDecimal();                   // 小数点
    void inputOperator(const QString &op); // 双目运算符 + - * /
    void calculateResult();                // 等号
    void backspace();                      // 退格
    void clearAll();                       // C：清除全部
    void clearEntry();                     // CE：只清当前操作数
    void toggleSign();                     // 正负号
    void inputPercent();                   // 百分号
    void applyReciprocal();                // 1/x
    void applySquare();                    // x²
    void applySqrt();                      // √x

    // ---- 内部辅助 ----
    bool    applyOperation(const QString &op, double lhs, double rhs, double &result) const;
    QString formatNumber(double value) const;
    void    refreshDisplay();
    void    setError(const QString &message);
    void    connectButtons();
    void    applyStyleSheet();

    // ---- 计算器状态 ----
    double  m_accumulator = 0.0;  // 第一个操作数或已计算的中间结果
    QString m_leftText;           // 左操作数在界面上的文本（保留原始输入格式）
    QString m_current;            // 当前正在输入 / 显示的操作数文本
    QString m_pendingOp;          // 待处理的运算符，空串表示没有
    bool    m_freshInput = true;  // 为真表示下一次输入将开始一个新的操作数
    bool    m_error = false;      // 错误状态（例如除数为 0）
    QString m_errorMessage;       // 错误提示文本

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
