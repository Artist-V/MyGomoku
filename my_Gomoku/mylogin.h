#ifndef MYLOGIN_H
#define MYLOGIN_H

#include <QWidget>
#include <mysql.h>

namespace Ui {
class mylogin;
}

class mylogin : public QWidget
{
    Q_OBJECT

public:
    explicit mylogin(QWidget *parent = 0);
    ~mylogin();

private slots:
    void on_RegisterButton_clicked();
    void on_playerButton_clicked();
    void on_LoginButton_clicked();
    void on_resetButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::mylogin *ui;
    mysql *sql;
};

#endif // MYLOGIN_H
