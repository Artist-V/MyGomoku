#ifndef ONLINEPKGAME_H
#define ONLINEPKGAME_H

#include <QMainWindow>
#include "gomokumodel.h"
#include "mysql.h"
#include <QHostAddress>

namespace Ui {
class onlinePKGame;
}

class QUdpSocket;
class onlinePKGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit onlinePKGame(QWidget *parent = 0);
    ~onlinePKGame();
private:
    Ui::onlinePKGame *ui;

private:
    mysql *sql;
    GomokuModel *game; // 游戏指针
    GameMode game_mode; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    QUdpSocket *udpSocket;
    qint16 port;
    qint16 targetport;
    bool step_flg; //棋权
    int myReady;   //我的准备情况
    int otherReady;//其他人准备情况
    bool isready;  //双方都准备好了
    bool isdown;   //玩家已经下好棋子了
    bool isconnect;//对方上线
    enum MessageType
    {
        Message,
        NewParticipant, //新玩家
        ParticipantLeft,//参与者列表
        Ready,          //游戏准备
        Gamesite,       //游戏结束
        GiveUp,         //认输
        DOWN            //已下棋
    };
    QString localHostName;
    QString address;
    QHostAddress addr;
    QString name;
private:
    void initGame();
    QString userName;
    QString uID;
    int type;
public:
    void paintEvent(QPaintEvent *event);    // 绘制
    void mouseMoveEvent(QMouseEvent *event);// 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子
    void initSocket();//初始化socket
    void participantLeft(QString userName, QString time);//处理玩家离开
    void sendMessage(MessageType type);//发送消息
    void newParticipant(QString userName,QString localHostName, QString ipAddress);//新玩家//////!!!!!!
    QString getMessage();//接收信息
    QPoint getGame();//获取坐标
    QString getIP();//获取IP
    QString getUserName();//获取主机名
    void getUser(QString uid,QString user,int Type);
    void initOther();
    int messagebox();
private slots:
    void readDatagram();//读取收到的数据报
    void on_backAction_triggered();
    void on_playAction_triggered();
    void on_computerAction_triggered();
    void on_saveAction_triggered();
    void on_readyButton_clicked();
    void on_failButton_clicked();
    void on_sendButton_clicked();
    void on_exitAction_triggered();
};

#endif // ONLINEPKGAME_H
