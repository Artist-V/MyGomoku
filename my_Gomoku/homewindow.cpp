#include "homewindow.h"
#include "ui_homewindow.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "computerpkgame.h"
#include "onlinepkgame.h"
#include "mylogin.h"
#include "userupdate.h"
#include "myregister.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_游戏主菜单");

    //sql初始化
    sql = new mysql;
    sql->connect_mysql();

    //标记初始化
    login_flg = true;
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::setLogin()
{
    login_flg = false;
}
int HomeWindow::messagebox_login()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("警告"),QStringLiteral("当前为游客登录模式，无法在线PK!\n是否注册游戏账号？"));
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    box.setButtonText (QMessageBox::Yes,QString(QStringLiteral("注册")));
    box.setButtonText (QMessageBox::No,QString(QStringLiteral("取消")));
    return box.exec();
}
int HomeWindow::messagebox_play()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("警告"),QStringLiteral("当前房间已满，无法进入在线PK"));
    box.setStandardButtons (QMessageBox::Yes);
    return box.exec();
}

void HomeWindow::getUser(QString uid,QString user)
{
    uID = uid;
    userName = user;
}
void HomeWindow::on_playPkButton_clicked()
{
    sql->close_mysql();
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
    this->close();
}
void HomeWindow::on_playAction_triggered()
{
    on_playPkButton_clicked();
}

void HomeWindow::on_computerPKButton_clicked()
{
    sql->close_mysql();
    this->close();
    int ret;
    ComputerPKGame *computerWin = new ComputerPKGame;
    computerWin->show();
    ret = computerWin->messagebox();
    if(ret == QMessageBox::Yes)
    {
        computerWin->userFirst();
    }
    else if(ret==QMessageBox::No)
    {
        computerWin->computerFirst();
    }
}
void HomeWindow::on_ComputerAction_triggered()
{
    on_computerPKButton_clicked();
}


void HomeWindow::on_HelpAction_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("   五子棋游戏玩法:\n"
                   "   对局双方一人执白棋，一人执黑棋（白棋方为先手）\n"
                   "   (1) 玩家PK：在本地机器上对战，一方倒计时结束即为失败；\n"
                   "   (2) 人机PK：与电脑对战，可以选择是玩家先手或电脑先手；\n"
                   "   (3) 在线PK：等待在线用户进入房间对战。\n");
    msgBox.exec();
}

void HomeWindow::on_onlinePKButton_clicked()
{
    if(sql->check_pkroom())
    {
        bool ret = sql->check_type(1);
        if(ret)
        {
            sql->update_type(uID,2);
            type = 2;
        }
        else
        {
            sql->update_type(uID,1);
            type = 1;
        }
        sql->close_mysql();
        this->close();

        onlinePKGame *onlineWindow = new onlinePKGame;
        onlineWindow->show();
        onlineWindow->getUser(uID,userName,type);
        onlineWindow->messagebox();
        onlineWindow->initOther();
    }
    else
    {
        int ret;
        ret  = messagebox_play();
        if(ret == QMessageBox::Yes)
        {
            this->close();
            myregister *reg = new myregister;
            reg->show();
        }
    }
}

void HomeWindow::on_OnlineAction_triggered()
{
    on_onlinePKButton_clicked();
}

void HomeWindow::on_ExitAction_triggered()
{
    sql->close_mysql();
    this->close();
    mylogin *loginWin = new mylogin;
    loginWin->show();
}

void HomeWindow::on_pushButton_clicked()
{
    sql->close_mysql();
    this->close();
    userUpdate *userup = new userUpdate;
    userup->show();
    userup->getUid(uID);
    userup->update();
}
