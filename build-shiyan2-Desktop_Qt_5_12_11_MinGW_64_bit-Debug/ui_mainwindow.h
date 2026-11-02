/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionAutoSave;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionFind;
    QAction *actionReplace;
    QAction *actionSelectAll;
    QAction *actionWordWrap;
    QAction *actionShowLineNumber;
    QAction *actionFont;
    QAction *actionTextColor;
    QAction *actionTextBackgroundColor;
    QAction *actionEditorBackgroundColor;
    QAction *actionShowToolBar;
    QAction *actionShowStatusBar;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    CodeEditor *editor;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuFormat;
    QMenu *menuView;
    QMenu *menuHelp;
    QToolBar *toolBar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 700);
        MainWindow->setWindowModified(false);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionAutoSave = new QAction(MainWindow);
        actionAutoSave->setObjectName(QString::fromUtf8("actionAutoSave"));
        actionAutoSave->setCheckable(true);
        actionAutoSave->setChecked(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName(QString::fromUtf8("actionFind"));
        actionReplace = new QAction(MainWindow);
        actionReplace->setObjectName(QString::fromUtf8("actionReplace"));
        actionSelectAll = new QAction(MainWindow);
        actionSelectAll->setObjectName(QString::fromUtf8("actionSelectAll"));
        actionWordWrap = new QAction(MainWindow);
        actionWordWrap->setObjectName(QString::fromUtf8("actionWordWrap"));
        actionWordWrap->setCheckable(true);
        actionWordWrap->setChecked(true);
        actionShowLineNumber = new QAction(MainWindow);
        actionShowLineNumber->setObjectName(QString::fromUtf8("actionShowLineNumber"));
        actionShowLineNumber->setCheckable(true);
        actionShowLineNumber->setChecked(true);
        actionFont = new QAction(MainWindow);
        actionFont->setObjectName(QString::fromUtf8("actionFont"));
        actionTextColor = new QAction(MainWindow);
        actionTextColor->setObjectName(QString::fromUtf8("actionTextColor"));
        actionTextBackgroundColor = new QAction(MainWindow);
        actionTextBackgroundColor->setObjectName(QString::fromUtf8("actionTextBackgroundColor"));
        actionEditorBackgroundColor = new QAction(MainWindow);
        actionEditorBackgroundColor->setObjectName(QString::fromUtf8("actionEditorBackgroundColor"));
        actionShowToolBar = new QAction(MainWindow);
        actionShowToolBar->setObjectName(QString::fromUtf8("actionShowToolBar"));
        actionShowToolBar->setCheckable(true);
        actionShowToolBar->setChecked(true);
        actionShowStatusBar = new QAction(MainWindow);
        actionShowStatusBar->setObjectName(QString::fromUtf8("actionShowStatusBar"));
        actionShowStatusBar->setCheckable(true);
        actionShowStatusBar->setChecked(true);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setMenuRole(QAction::AboutRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        editor = new CodeEditor(centralwidget);
        editor->setObjectName(QString::fromUtf8("editor"));

        verticalLayout->addWidget(editor);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFormat = new QMenu(menubar);
        menuFormat->setObjectName(QString::fromUtf8("menuFormat"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuFormat->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionAutoSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFind);
        menuEdit->addAction(actionReplace);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelectAll);
        menuFormat->addAction(actionWordWrap);
        menuFormat->addAction(actionShowLineNumber);
        menuFormat->addSeparator();
        menuFormat->addAction(actionFont);
        menuFormat->addAction(actionTextColor);
        menuFormat->addAction(actionTextBackgroundColor);
        menuFormat->addAction(actionEditorBackgroundColor);
        menuView->addAction(actionShowToolBar);
        menuView->addAction(actionShowStatusBar);
        menuHelp->addAction(actionAbout);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionSaveAs);
        toolBar->addSeparator();
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        toolBar->addSeparator();
        toolBar->addAction(actionCut);
        toolBar->addAction(actionCopy);
        toolBar->addAction(actionPaste);
        toolBar->addSeparator();
        toolBar->addAction(actionFind);
        toolBar->addAction(actionReplace);
        toolBar->addSeparator();
        toolBar->addAction(actionFont);
        toolBar->addAction(actionShowLineNumber);
        toolBar->addAction(actionTextColor);
        toolBar->addAction(actionTextBackgroundColor);
        toolBar->addAction(actionEditorBackgroundColor);
        toolBar->addSeparator();
        toolBar->addAction(actionWordWrap);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\226\207\346\234\254\346\226\207\344\273\266[*] - \347\274\226\350\276\221\345\231\250", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)", nullptr));
        actionNew->setIconText(QApplication::translate("MainWindow", "\346\226\260\345\273\272", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\226\207\344\273\266 (Ctrl+N)", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)...", nullptr));
        actionOpen->setIconText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266 (Ctrl+O)", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230(&S)", nullptr));
        actionSave->setIconText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\226\207\344\273\266 (Ctrl+S)", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSaveAs->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(&A)...", nullptr));
        actionSaveAs->setIconText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSaveAs->setToolTip(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272 (Ctrl+Shift+S)", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionAutoSave->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\344\277\235\345\255\230(&U)", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&X)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionUndo->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200(&U)", nullptr));
        actionUndo->setIconText(QApplication::translate("MainWindow", "\346\222\244\351\224\200", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("MainWindow", "\346\201\242\345\244\215(&R)", nullptr));
        actionRedo->setIconText(QApplication::translate("MainWindow", "\346\201\242\345\244\215", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("MainWindow", "\345\211\252\345\210\207(&T)", nullptr));
        actionCut->setIconText(QApplication::translate("MainWindow", "\345\211\252\345\210\207", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy->setText(QApplication::translate("MainWindow", "\345\244\215\345\210\266(&C)", nullptr));
        actionCopy->setIconText(QApplication::translate("MainWindow", "\345\244\215\345\210\266", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("MainWindow", "\347\262\230\350\264\264(&P)", nullptr));
        actionPaste->setIconText(QApplication::translate("MainWindow", "\347\262\230\350\264\264", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionFind->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276(&F)...", nullptr));
        actionFind->setIconText(QApplication::translate("MainWindow", "\346\237\245\346\211\276", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFind->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_NO_SHORTCUT
        actionReplace->setText(QApplication::translate("MainWindow", "\346\233\277\346\215\242(&R)...", nullptr));
        actionReplace->setIconText(QApplication::translate("MainWindow", "\346\233\277\346\215\242", nullptr));
#ifndef QT_NO_SHORTCUT
        actionReplace->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelectAll->setText(QApplication::translate("MainWindow", "\345\205\250\351\200\211(&A)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSelectAll->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        actionWordWrap->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\215\242\350\241\214(&W)", nullptr));
        actionShowLineNumber->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\350\241\214\345\217\267(&L)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionShowLineNumber->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_NO_SHORTCUT
        actionFont->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223(&F)...", nullptr));
        actionFont->setIconText(QApplication::translate("MainWindow", "\345\255\227\344\275\223", nullptr));
        actionTextColor->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223\351\242\234\350\211\262(&C)...", nullptr));
        actionTextColor->setIconText(QApplication::translate("MainWindow", "\345\255\227\344\275\223\351\242\234\350\211\262", nullptr));
        actionTextBackgroundColor->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223\350\203\214\346\231\257\350\211\262(&B)...", nullptr));
        actionTextBackgroundColor->setIconText(QApplication::translate("MainWindow", "\345\255\227\344\275\223\350\203\214\346\231\257\350\211\262", nullptr));
        actionEditorBackgroundColor->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221\345\231\250\350\203\214\346\231\257\350\211\262(&E)...", nullptr));
        actionEditorBackgroundColor->setIconText(QApplication::translate("MainWindow", "\347\274\226\350\276\221\345\231\250\350\203\214\346\231\257\350\211\262", nullptr));
        actionShowToolBar->setText(QApplication::translate("MainWindow", "\345\267\245\345\205\267\346\240\217(&T)", nullptr));
        actionShowStatusBar->setText(QApplication::translate("MainWindow", "\347\212\266\346\200\201\346\240\217(&S)", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216(&A)...", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", nullptr));
        menuFormat->setTitle(QApplication::translate("MainWindow", "\346\240\274\345\274\217(&O)", nullptr));
        menuView->setTitle(QApplication::translate("MainWindow", "\346\237\245\347\234\213(&V)", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "\345\267\245\345\205\267\346\240\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
