/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *BackAction;
    QAction *ExitAction;
    QAction *playerPKAction;
    QAction *computerPKAction;
    QAction *SaveAction;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QLabel *blackTime;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_3;
    QLabel *whiteTime;
    QMenuBar *menuBar;
    QMenu *menu_A;
    QMenu *menu;
    QMenu *menu_P;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(850, 660);
        BackAction = new QAction(MainWindow);
        BackAction->setObjectName(QStringLiteral("BackAction"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/home.png"), QSize(), QIcon::Normal, QIcon::Off);
        BackAction->setIcon(icon);
        ExitAction = new QAction(MainWindow);
        ExitAction->setObjectName(QStringLiteral("ExitAction"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        ExitAction->setIcon(icon1);
        playerPKAction = new QAction(MainWindow);
        playerPKAction->setObjectName(QStringLiteral("playerPKAction"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/person.png"), QSize(), QIcon::Normal, QIcon::Off);
        playerPKAction->setIcon(icon2);
        computerPKAction = new QAction(MainWindow);
        computerPKAction->setObjectName(QStringLiteral("computerPKAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        computerPKAction->setIcon(icon3);
        SaveAction = new QAction(MainWindow);
        SaveAction->setObjectName(QStringLiteral("SaveAction"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/screenshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        SaveAction->setIcon(icon4);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(660, 0, 131, 531));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\347\273\206\351\273\221"));
        font.setPointSize(20);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        blackTime = new QLabel(verticalLayoutWidget);
        blackTime->setObjectName(QStringLiteral("blackTime"));
        blackTime->setFont(font);
        blackTime->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(blackTime);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        whiteTime = new QLabel(verticalLayoutWidget);
        whiteTime->setObjectName(QStringLiteral("whiteTime"));
        whiteTime->setFont(font);
        whiteTime->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(whiteTime);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 850, 26));
        menu_A = new QMenu(menuBar);
        menu_A->setObjectName(QStringLiteral("menu_A"));
        menu = new QMenu(menu_A);
        menu->setObjectName(QStringLiteral("menu"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        menu->setIcon(icon5);
        menu_P = new QMenu(menuBar);
        menu_P->setObjectName(QStringLiteral("menu_P"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_A->menuAction());
        menuBar->addAction(menu_P->menuAction());
        menu_A->addAction(menu->menuAction());
        menu_A->addAction(BackAction);
        menu_A->addAction(ExitAction);
        menu->addAction(playerPKAction);
        menu->addAction(computerPKAction);
        menu_P->addAction(SaveAction);

        retranslateUi(MainWindow);
        QObject::connect(ExitAction, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        BackAction->setText(QApplication::translate("MainWindow", "\350\277\224\345\233\236\344\270\273\350\217\234\345\215\225", 0));
        ExitAction->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\346\270\270\346\210\217", 0));
        playerPKAction->setText(QApplication::translate("MainWindow", "\347\216\251\345\256\266PK", 0));
        computerPKAction->setText(QApplication::translate("MainWindow", "\344\272\272\346\234\272PK", 0));
        SaveAction->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\243\213\345\261\200", 0));
        label->setText(QApplication::translate("MainWindow", "\345\200\222\350\256\241\346\227\266", 0));
        blackTime->setText(QApplication::translate("MainWindow", "05-00", 0));
        label_3->setText(QApplication::translate("MainWindow", "\345\200\222\350\256\241\346\227\266 ", 0));
        whiteTime->setText(QApplication::translate("MainWindow", "05-00", 0));
        menu_A->setTitle(QApplication::translate("MainWindow", "\345\274\200\345\247\213\357\274\210&A\357\274\211", 0));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\260\347\232\204\344\270\200\345\261\200", 0));
        menu_P->setTitle(QApplication::translate("MainWindow", "\346\270\270\346\210\217\357\274\210&P\357\274\211", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
