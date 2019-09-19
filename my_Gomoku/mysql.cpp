#include "mysql.h"
#include <QDebug>

mysql::mysql()
{
}

void mysql::connect_mysql()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("mygomoku");
    db.setUserName("root");
    db.setPassword("123456");
    db.open();
}
void mysql::close_mysql()
{
    db.close();
}
void mysql::create_table()
{
    QSqlQuery query(db);
    query.exec("create table if not exists user(uid char(15) PRIMARY KEY NOT NULL , "
               "upwd char(15) NOT NULL, uname char(15) NOT NULL,type int NOT NULL ,play int NOT NULL)");
}

bool mysql::check_uid(QString uid, QString upwd)
{
    QSqlQuery query(db);
    QString sql = QString("select * from user where uid = '%1' and upwd = '%2'").arg(uid).arg(upwd);
    query.exec(sql);
    query.next();
    if((query.at()) == 0)  //query指向的记录在结果集中的编号
    {
        return true;
    }
    else
    {
        return false;
    }
}

void mysql::insert_uid(QString uid, QString upwd,QString uname,int type,int play)
{
    QSqlQuery query(db);
    QString sql = QString("insert into user values('%1','%2','%3','%4','%5')").arg(uid).arg(upwd).arg(uname).arg(type).arg(play);
    query.exec(sql);
}
bool mysql::check_insert(QString uid)
{
    QSqlQuery query(db);
    QString sql = QString("select * from user where uid='%1'").arg(uid);
    query.exec(sql);
    query.next();
    if((query.at()) == 0)  //query指向的记录在结果集中的编号
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString mysql::getName(QString uid)
{
    QSqlQuery query(db);
    QString sql = QString("select uname from user where uid = '%1'").arg(uid);
    query.exec(sql);
    QSqlRecord ret = query.record();
    while(query.next())
    {
        ret = query.record();
        int snocol = ret.indexOf("uname");
        user = query.value(snocol).toString();
    }
    return user;
}
QString mysql::getPwd(QString uid)
{
    QSqlQuery query(db);
    QString sql = QString("select upwd from user where uid = '%1'").arg(uid);
    query.exec(sql);
    QSqlRecord ret = query.record();
    while(query.next())
    {
        ret = query.record();
        int snocol = ret.indexOf("upwd");
        upwd = query.value(snocol).toString();
    }
    return upwd;
}

void mysql::update_type(QString uid,int value)
{
    QSqlQuery query(db);
    QString sql = QString("update user set type='%1' where uid='%2'").arg(value).arg(uid);
    query.exec(sql);
}
bool mysql::check_type(int value)
{
    QSqlQuery query(db);
    QString sql = QString("select * from user where type='%1'").arg(value);
    query.exec(sql);
    query.next();
    if((query.at()) == 0)  //无结果 / query指向的记录在结果集中的编号
    {
        return true;
    }
    else
    {
        return false;
    }
}
void mysql::update_name(QString uid,QString name)
{
    QSqlQuery query(db);
    QString sql = QString("update user set uname='%1' where uid='%2'").arg(name).arg(uid);
    query.exec(sql);
}

void mysql::update_pwd(QString uid,QString pwd)
{
    QSqlQuery query(db);
    QString sql = QString("update user set upwd='%1' where uid='%2'").arg(pwd).arg(uid);
    query.exec(sql);
}
bool mysql::check_pkroom()
{
    QSqlQuery query(db);
    QString sql = QString("select * from user where type='1'");
    query.exec(sql);
    query.next();
    if((query.at()) == 0)  //无结果 / query指向的记录在结果集中的编号
    {
        qDebug()<<"type == true";
        return true;
    }
    else
    {

        qDebug()<<"type == false";
        return false;
    }
}
void mysql::update_play(QString uid)
{
    QSqlQuery query(db);
    QString sql = QString("update user set play='1' where uid='%1'").arg(uid);
    query.exec(sql);
}

//房间是否满，true=满，false=不满
bool mysql::check_play()
{
    QSqlQuery query(db);
    QString sql = QString("select * from user where play='1'");
    query.exec(sql);
    query.next();
    if((query.seek(1)) == 1)  // query指向的记录在结果集中的编号
    {
        return false;
    }
    else
    {
        return true;
    }
}
void mysql::reset_play(QString uid)
{
    QSqlQuery query(db);
    QString sql = QString("update user set play='0' where uid='%1'").arg(uid);
    query.exec(sql);
}

