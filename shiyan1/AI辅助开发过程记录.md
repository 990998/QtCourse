AI辅助开发过程记录（实验一：Qt计算器——键盘输入支持）

1. 问题
计算器鼠标点击已正常，但无法用键盘输入。需支持数字、+ − × ÷、小数点、回车、退格、Esc，且键盘与鼠标必须共用同一套计算逻辑。

2. 提示词
1）用 Qt5 写一个计算器，QGridLayout 排 6 行 4 列按钮，支持加减乘除、等号，并支持键盘输入，计算逻辑与界面分开。
2）键盘有些键没反应，回车会触发当前焦点按钮，退格也失效，检查一下焦点和快捷键的问题。

3. AI给出的方案
按钮用 clicked 连接各输入函数；键盘用 QShortcut + QKeySequence 给每个键注册快捷键，并把显示框设为 readOnly。

4. 实际运行结果
鼠标操作全部正常；数字键、小数点可用；* 和 / 时好时坏；+ 和 − 需按 Shift，常匹配失败；回车无反应或触发焦点按钮；Backspace 无效。

5. 存在的问题
1）QPushButton 默认可获得焦点，Tab、空格、回车会被按钮吃掉。
2）QShortcut 与 clicked 是两套输入路径，状态容易不同步。
3）QLineEdit 即使 readOnly 仍能获得焦点，会吞掉 Backspace/Delete。
4）QKeySequence 对符号键的匹配受键盘布局和 Shift 影响，不可靠。

6. 自己的修改
1）遍历所有按钮并设置 setFocusPolicy(Qt::NoFocus)，显示框也同样处理。
2）删除全部 QShortcut，改为重写 keyPressEvent，通过 event->key() 分发，只调用与鼠标相同的 inputXxx() 方法，不重复实现逻辑。
3）把 Return/Enter/Equal 归一到 calculateResult，Period/Comma 归一到 inputDecimal，Delete 归一到 clearEntry，Escape 归一到 clearAll。
4）未识别的按键交回基类 QMainWindow::keyPressEvent，避免吞掉系统快捷键。

7. 修改后结果
Qt 5.12.11 MinGW 64 位下编译 0 error、0 warning。键盘输入 12 + 3 回车得 15；0.1 + 0.2 显示 0.3；8 ÷ 0 提示“除数不能为 0”；鼠标与键盘行为完全一致。

8. 遗留问题
乘号 *、百分号 % 在部分键盘布局下仍需 Shift；未支持括号、科学计数、复制粘贴；尚无自动化单元测试。
