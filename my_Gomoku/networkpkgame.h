#ifndef NETWORKPKGAME_H
#define NETWORKPKGAME_H

#include <QMainWindow>
#include "gomokumodel.h"

namespace Ui {
class networkPKGame;
}

class QUdpSocket;
class networkPKGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit networkPKGame(QWidget *parent = 0);
    ~networkPKGame();
private:
    Ui::networkPKGame *ui;

private:
    GomokuModel *game; // 游戏指针
    GameMode game_mode; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    QUdpSocket *udpSocket;
    qint16 port;
    int firstReady;//第一个按准备的
    int myReady;   //我的准备情况
    int otherReady;//其他人准备情况
    bool isready;  //双方都准备好了
    bool isdown;   //玩家已经下好棋子了
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
private:
    void initGame();
public:
    void paintEvent(QPaintEvent *event);    // 绘制
    void mouseMoveEvent(QMouseEvent *event);// 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子
    void initSocket();//初始化socket
    void participantLeft(QString userName, QString time);//获取参与者列表
    void sendMessage(MessageType type);//发送消息
    void newParticipant(QString userName, QString ipAddress);//新玩家
    QString getMessage();//接收信息
    QPoint getGame();//获取坐标
    QString getIP();//获取IP
    QString getUserName();//获取主机名
private slots:
    void processPendingDatagrams();//处理数据
    void on_backAction_triggered();
    void on_playerAction_triggered();
    void on_computerAction_triggered();
    void on_onlineAction_triggered();
    void on_saveAction_triggered();
    void on_readyButton_clicked();
    void on_failButton_clicked();
    void on_sendButton_clicked();
};

#endif // NETWORKPKGAME_H
