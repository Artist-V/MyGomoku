#ifndef MYREGISTER_H
#define MYREGISTER_H

#include <QWidget>
#include <mysql.h>

namespace Ui {
class myregister;
}

class myregister : public QWidget
{
    Q_OBJECT

public:
    explicit myregister(QWidget *parent = 0);
    ~myregister();

private slots:
    void on_registerButton_clicked();
    void on_resetButton_clicked();
    void on_ExitButton_clicked();

private:
    Ui::myregister *ui;
    mysql *sql;
};

#endif // MYREGISTER_H
