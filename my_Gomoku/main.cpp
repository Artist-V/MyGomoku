#include <QApplication>
#include "mylogin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mylogin *loginWin = new mylogin;
    loginWin->show();

    return a.exec();
}
