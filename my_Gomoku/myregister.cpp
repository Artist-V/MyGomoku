#include "myregister.h"
#include "ui_myregister.h"
#include "mylogin.h"
#include <QMessageBox>

myregister::myregister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myregister)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_玩家注册");

    //设置
    this->setFixedSize(this->width(),this->height());//窗口大小固定
    ui->PwdlineEdit->setEchoMode(QLineEdit::Password);
    ui->PwdlineEdit_2->setEchoMode(QLineEdit::Password);

    // 初始化sql
    sql = new mysql();
    sql->connect_mysql();
}

myregister::~myregister()
{
    delete ui;
    sql->close_mysql();
}

void myregister::on_registerButton_clicked()
{
    QString uid = ui->UserlineEdit->text();
    QString upwd1 = ui->PwdlineEdit->text();
    QString upwd2 = ui->PwdlineEdit_2->text();
    QString uname = ui->NamelineEdit->text();

    if(uid == "" || upwd1 == "" || upwd2 == "" || uname == "")
    {
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    }
    else if(QString::compare(upwd1,upwd2) != 0)
    {
        QMessageBox::information(this,"警告","两次密码输入不相同",QMessageBox::Ok);
        ui->PwdlineEdit->setText("");
        ui->PwdlineEdit_2->setText("");
    }
    else if(sql->check_insert(uid))
    {
        QMessageBox::information(this,"警告","该用户已被注册",QMessageBox::Ok);
        ui->UserlineEdit->setText("");
        ui->PwdlineEdit->setText("");
        ui->PwdlineEdit_2->setText("");
    }
    else
    {
        sql->insert_uid(uid,upwd1,uname,0,0);
        QMessageBox::information(this,"提示","注册成功",QMessageBox::Ok);
        this->close();
        mylogin *login = new mylogin;
        login->show();
    }
}

void myregister::on_resetButton_clicked()
{
    ui->UserlineEdit->setText("");
    ui->PwdlineEdit->setText("");
    ui->PwdlineEdit_2->setText("");
    ui->NamelineEdit->setText("");
}

void myregister::on_ExitButton_clicked()
{
    this->close();
    sql->close_mysql();
}
