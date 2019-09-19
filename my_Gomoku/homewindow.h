#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H
#include "mysql.h"
#include <QMainWindow>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

public:
    void getUser(QString uid,QString user);
    void setLogin();
    int messagebox_login();
    int messagebox_play();
private:
    QString userName;
    QString uID;
    mysql *sql;
    int type;
    bool login_flg;

private slots:
    void on_playPkButton_clicked();
    void on_computerPKButton_clicked();
    void on_HelpAction_triggered();
    void on_playAction_triggered();
    void on_ComputerAction_triggered();
    void on_onlinePKButton_clicked();
    void on_OnlineAction_triggered();
    void on_ExitAction_triggered();
    void on_pushButton_clicked();

private:
    Ui::HomeWindow *ui;
};

#endif // HOMEWINDOW_H
