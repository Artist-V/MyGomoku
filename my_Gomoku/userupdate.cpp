#include "userupdate.h"
#include "ui_userupdate.h"
#include <QMessageBox>

userUpdate::userUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userUpdate)
{
    ui->setupUi(this);
    this->setWindowTitle("修改个人信息");

    sql = new mysql;
}

userUpdate::~userUpdate()
{
    delete ui;
}
void userUpdate::update()
{
    QString pwd = sql->getPwd(uID);
    ui->PwdlineEdit_2->setText(pwd);
    QString name = sql->getName(uID);
    ui->namelineEdit->setText(name);
}

void userUpdate::getUid(QString uid)
{
    uID = uid;
}

void userUpdate::on_pwdUpdateButton_clicked()
{
    QString pwd = ui->PwdlineEdit_2->text();
    sql->update_pwd(uID,pwd);
    int ret = messagebox();
    if(ret == QMessageBox::No)
    {
        this->close();
    }
}

void userUpdate::on_nameUpdateButton_clicked()
{
    QString name = ui->namelineEdit->text();
    sql->update_name(uID,name);
    int ret = messagebox();
    if(ret == QMessageBox::No)
    {
        this->close();
    }
}

void userUpdate::on_resetButton_2_clicked()
{
    ui->PwdlineEdit_2->setText("");
    ui->namelineEdit->setText("");
}

void userUpdate::on_ExitButton_2_clicked()
{
    sql->close_mysql();
    this->close();
}
int userUpdate::messagebox()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("提示"),QStringLiteral("修改成功,是否继续修改?"));
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    box.setButtonText (QMessageBox::Yes,QString(QStringLiteral("确定")));
    box.setButtonText (QMessageBox::No,QString(QStringLiteral("取消")));
    return box.exec();
}

