#ifndef MYSQL_H
#define MYSQL_H
#pragma execution_character_set("utf-8")
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QtDebug>

class mysql
{
public:
    mysql();
    void connect_mysql();// 连接数据库
    void close_mysql();// 关闭数据库
    void create_table();//创建表
    bool check_uid(QString uid, QString upwd);//登录时：检查用户账号密码是否有效
    void insert_uid(QString uid, QString upwd,QString uname,int type,int play);//插入新用户
    bool check_insert(QString uid);//注册时：检查uid是否存在
    QString getName(QString uid);//取昵称
    QString getPwd(QString uid);//取密码
    void update_type(QString uid,int value);//更新用户在线PK状态
    bool check_type(int value);//在线PK：检查状态的数值
    void update_name(QString uid,QString name);//修改个人信息：修改昵称
    void update_pwd(QString uid,QString pwd);//修改个人信息：修改密码
    bool check_pkroom();//主界面：检查在线pk房间是否已满
    void update_play(QString uid);//设置play = 1
    bool check_play();
    void reset_play(QString uid);//设置play = 0

private:
     QSqlDatabase db;
     QString user;
     QString upwd;

};

#endif // MYSQL_H
