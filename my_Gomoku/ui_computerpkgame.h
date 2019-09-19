/********************************************************************************
** Form generated from reading UI file 'computerpkgame.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPUTERPKGAME_H
#define UI_COMPUTERPKGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComputerPKGame
{
public:
    QAction *backAction;
    QAction *exitAction;
    QAction *saveAction;
    QAction *playPKAction;
    QAction *computerPKAction;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu_A;
    QMenu *menu;
    QMenu *menu_P;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ComputerPKGame)
    {
        if (ComputerPKGame->objectName().isEmpty())
            ComputerPKGame->setObjectName(QStringLiteral("ComputerPKGame"));
        ComputerPKGame->resize(655, 600);
        backAction = new QAction(ComputerPKGame);
        backAction->setObjectName(QStringLiteral("backAction"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/home.png"), QSize(), QIcon::Normal, QIcon::Off);
        backAction->setIcon(icon);
        exitAction = new QAction(ComputerPKGame);
        exitAction->setObjectName(QStringLiteral("exitAction"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitAction->setIcon(icon1);
        saveAction = new QAction(ComputerPKGame);
        saveAction->setObjectName(QStringLiteral("saveAction"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/screenshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveAction->setIcon(icon2);
        playPKAction = new QAction(ComputerPKGame);
        playPKAction->setObjectName(QStringLiteral("playPKAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/person.png"), QSize(), QIcon::Normal, QIcon::Off);
        playPKAction->setIcon(icon3);
        computerPKAction = new QAction(ComputerPKGame);
        computerPKAction->setObjectName(QStringLiteral("computerPKAction"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        computerPKAction->setIcon(icon4);
        centralwidget = new QWidget(ComputerPKGame);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        ComputerPKGame->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ComputerPKGame);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 655, 26));
        menu_A = new QMenu(menubar);
        menu_A->setObjectName(QStringLiteral("menu_A"));
        menu = new QMenu(menu_A);
        menu->setObjectName(QStringLiteral("menu"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        menu->setIcon(icon5);
        menu_P = new QMenu(menubar);
        menu_P->setObjectName(QStringLiteral("menu_P"));
        ComputerPKGame->setMenuBar(menubar);
        statusbar = new QStatusBar(ComputerPKGame);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ComputerPKGame->setStatusBar(statusbar);

        menubar->addAction(menu_A->menuAction());
        menubar->addAction(menu_P->menuAction());
        menu_A->addAction(menu->menuAction());
        menu_A->addAction(backAction);
        menu_A->addAction(exitAction);
        menu->addAction(playPKAction);
        menu->addAction(computerPKAction);
        menu_P->addAction(saveAction);

        retranslateUi(ComputerPKGame);
        QObject::connect(exitAction, SIGNAL(triggered()), ComputerPKGame, SLOT(close()));

        QMetaObject::connectSlotsByName(ComputerPKGame);
    } // setupUi

    void retranslateUi(QMainWindow *ComputerPKGame)
    {
        ComputerPKGame->setWindowTitle(QApplication::translate("ComputerPKGame", "MainWindow", 0));
        backAction->setText(QApplication::translate("ComputerPKGame", "\350\277\224\345\233\236\344\270\273\350\217\234\345\215\225", 0));
        exitAction->setText(QApplication::translate("ComputerPKGame", "\351\200\200\345\207\272\346\270\270\346\210\217", 0));
        saveAction->setText(QApplication::translate("ComputerPKGame", "\344\277\235\345\255\230\346\243\213\345\261\200", 0));
        playPKAction->setText(QApplication::translate("ComputerPKGame", "\347\216\251\345\256\266PK", 0));
        computerPKAction->setText(QApplication::translate("ComputerPKGame", "\344\272\272\346\234\272PK", 0));
        menu_A->setTitle(QApplication::translate("ComputerPKGame", "\345\274\200\345\247\213\357\274\210&A)", 0));
        menu->setTitle(QApplication::translate("ComputerPKGame", "\346\226\260\347\232\204\344\270\200\345\261\200", 0));
        menu_P->setTitle(QApplication::translate("ComputerPKGame", "\346\270\270\346\210\217\357\274\210&P\357\274\211", 0));
    } // retranslateUi

};

namespace Ui {
    class ComputerPKGame: public Ui_ComputerPKGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPUTERPKGAME_H
