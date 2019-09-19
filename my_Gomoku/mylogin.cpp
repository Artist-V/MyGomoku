#include "mylogin.h"
#include "ui_mylogin.h"
#include "myregister.h"
#include "mainwindow.h"
#include "homewindow.h"
#include <QMessageBox>

mylogin::mylogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mylogin)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_玩家登录");

    //初始化sql
    sql = new mysql;
    sql->connect_mysql();
    sql->create_table();

}

mylogin::~mylogin()
{
    delete ui;
    sql->close_mysql();
}

void mylogin::on_RegisterButton_clicked()
{
    this->close();
    sql->close_mysql();
    myregister *registerWin = new myregister;
    registerWin->show();
}

void mylogin::on_playerButton_clicked()
{
    this->close();
    sql->close_mysql();
    HomeWindow *homeWin = new HomeWindow;
    homeWin->setLogin();
    homeWin->show();
}

void mylogin::on_LoginButton_clicked()
{
    QString uid  = ui->UserlineEdit->text();
    QString upwd = ui->PwdlineEdit->text();
    if(uid == "" || upwd == "")
    {
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    }
    else if(sql->check_uid(uid,upwd))
    {
        HomeWindow *homeWin = new HomeWindow;
        homeWin->getUser(uid,sql->getName(uid));
        homeWin->show();
        sql->close_mysql();
        this->close();
    }
    else
    {
        QMessageBox::information(this,"错误","用户名或密码错误",QMessageBox::Ok);
    }
}

void mylogin::on_resetButton_clicked()
{
    ui->UserlineEdit->setText("");
    ui->PwdlineEdit->setText("");
}

void mylogin::on_ExitButton_clicked()
{
    this->close();
    //sql->update_type(0);
    sql->close_mysql();
}
