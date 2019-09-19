#ifndef USERUPDATE_H
#define USERUPDATE_H

#include <QWidget>
#include "mysql.h"

namespace Ui {
class userUpdate;
}

class userUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit userUpdate(QWidget *parent = 0);
    ~userUpdate();
    void getUid(QString uid);
    void update();
    int messagebox();

private slots:
    void on_pwdUpdateButton_clicked();
    void on_resetButton_2_clicked();
    void on_ExitButton_2_clicked();
    void on_nameUpdateButton_clicked();

private:
    Ui::userUpdate *ui;
    mysql *sql;
    QString uID;
};

#endif // USERUPDATE_H
