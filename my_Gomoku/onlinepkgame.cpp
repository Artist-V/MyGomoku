#include "onlinepkgame.h"
#include "ui_onlinepkgame.h"
#include "mainwindow.h"
#include "computerpkgame.h"
#include "homewindow.h"
#include <QMessageBox>
#include <QUdpSocket>
#include <QHostInfo>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QFileDialog>
#include <QPainter>
#include <QTimer>
#include <qdebug.h>

/********* 全局属性  **********************************/
const int kBoard = 600;
const int kBoardWidth = 1100;
const int kBoardHeight = 660;
const int kBoardMargin = 40; // 棋盘边缘空隙30
const int kRadius = 15;      // 棋子半径
const int kMarkSize = 6;     // 落子标记边长
const int kBlockSize = 40;   // 格子的大小
const int kPosDelta = 20;    // 鼠标点击的模糊距离上限
const int kAIDelay = 700;    // AI下棋的思考时间
/****************************************************/

onlinePKGame::onlinePKGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::onlinePKGame)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_在线PK");

    // 设置面板颜色
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);

    // 设置棋盘大小
    this->setFixedSize(kBoardWidth,kBoardHeight);
    this->setAutoFillBackground(true);

    // 开启鼠标hover功能
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);

    // sql初始化
    sql = new mysql();
    sql->connect_mysql();

    // 标记初始化
    myReady = 0;
    otherReady = 0;
    isready = false;
    isdown = false;
    step_flg = false;
    isconnect = false;
    name = getUserName();
    localHostName = QHostInfo::localHostName();//主机名
    address = "127.0.0.1";
            //getIP();//地址
    addr = "127.0.0.1";

    // 游戏初始化
    initGame();
}

onlinePKGame::~onlinePKGame()
{
    delete ui;
	delete udpSocket;
    sql->close_mysql();
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void onlinePKGame::initSocket()
{
    udpSocket = new QUdpSocket(this);
}

void onlinePKGame::initGame()
{
    game = new GomokuModel;
    game_mode = ONLINE;
    game->playType = PLAYING;
    game->start(game_mode);
    update();
}
void onlinePKGame::initOther()
{
    //socket初始化
    initSocket();

    qDebug()<<"type"<<type;
    //状态
    if(type == 1)
    {
        port = 9999;    // 端口号
        targetport = 8888;// 对方端口号
    }
    else
    {
        port = 8888;    // 端口号
        targetport = 9999;// 对方端口号
    }

    if(udpSocket->bind(port))
    {
        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readDatagram()));
    }
    else
    {
        qDebug()<<"socket绑定失败";
    }

    //发送在线消息
    sendMessage(NewParticipant);

    //进入游戏的提示
    newParticipant(localHostName,userName,address);

    ui->messageBrowser->append(QStringLiteral("您的 port : %2").arg(port));
    ui->messageBrowser->append(QStringLiteral("对方 port : %2").arg(targetport));
}
int onlinePKGame::messagebox()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("提示"),QStringLiteral("请等待其他玩家进入游戏"));
    box.setStandardButtons (QMessageBox::Yes);
    box.setButtonText (QMessageBox::Yes,QString(QStringLiteral("确定")));
    return box.exec();
}

//读取收到的数据报
void onlinePKGame::readDatagram()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in >> messageType;

        QString username,localHostName,ipAddress,message;
        QPoint gamesite;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QMessageBox::StandardButton button;

        switch(messageType)
        {
            case Message:
                in >> username >> message;
                ui->messageBrowser->setTextColor(Qt::blue);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",11));
                ui->messageBrowser->append("["+username+"]"+time);
                ui->messageBrowser->append(message);
                qDebug()<<"read : Message";
                break;

            // 新玩家
            case NewParticipant:
                in >> localHostName >> username>> ipAddress;
                newParticipant(localHostName,username,ipAddress);////////!!!!!!
                //有新玩家进入游戏，告诉他自己也在线
                if(isconnect == false)
                {
                    sendMessage(NewParticipant);
                    isconnect = true;
                }
                break;

            // 玩家离开
            case ParticipantLeft:
                in >> username ;
                participantLeft(username,time);
                qDebug()<<"read : ParticipantLeft";
                isconnect = false;
                break;

            // 认输
            case GiveUp:
                if(otherReady == 1)
                {
                    button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("白棋认输,黑棋获胜"));
                }
                else
                {
                    button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("黑棋认输,白棋获胜"));
                }
                // 重置游戏状态
                if (button == QMessageBox::Ok)
                {
                    game->start(game_mode);
                    game->playType = PLAYING;
                    ui->readyButton->setEnabled(true);
                }
                qDebug()<<"read : GiveUp";
                break;

            // 游戏准备(先准备好的：白棋；后准备好的黑棋)
            case Ready:
                in >> username;
            // 是玩家先准备好了
                if(myReady == 0)
                {
                    otherReady = 1;
                }
            // 是对手先准备好了
                else
                {
                    otherReady = 2;
                }

                ui->messageBrowser->setTextColor(Qt::black);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->messageBrowser->append(QStringLiteral("%1准备").arg(username));

                qDebug()<<"myReady = "<<myReady;
                qDebug()<<"otherReady = "<<otherReady;

                if(myReady != 0&& otherReady != 0)
                {
                    if(myReady < otherReady)
                    {
//game->chess_flag = true;//白棋
                        step_flg = true;
                    }
                    qDebug()<<"step_flg = ture";
//else
//{
//    game->chess_flag = false;//黑棋
//}
                    update();
                    ui->messageBrowser->append(QStringLiteral("游戏开始!"));
                    isready = true;
                    qDebug()<< "游戏开始!";
                }
                qDebug()<<"read : Ready";
                break;

            // 游戏
            case Gamesite:
                in >> username >> gamesite;
                clickPosRow = gamesite.x();
                clickPosCol = gamesite.y();
                game->PlayWithPerson(gamesite.x(),gamesite.y());
                update();
                if(game->isWin(gamesite.x(),gamesite.y()))
                {
                    game->playType = WIN;
                    if(game->gameMapVec[clickPosRow][clickPosCol] == 1)
                    {
                        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
                    }
                    else
                    {
                        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
                    }
                }
                else
                {
                    step_flg = true;
                }
                // 重置游戏状态
                if (button == QMessageBox::Ok)
                {
                    game->start(game_mode);
                    game->playType = PLAYING;

                    // 按钮
                    ui->readyButton->setEnabled(true);
                    // 标记初始化
                    myReady = 0;
                    otherReady = 0;
                    isready = false;
                    isdown = false;
                    step_flg = false;
                }
                qDebug()<<"read : Gamesite";
                break;

            case DOWN:
                in >>  username;
                isdown = false;
                qDebug()<<"read : DOWN";
                break;
        }
    }
}

void onlinePKGame::sendMessage(MessageType type)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << type ;
    switch(type)
    {
        // 消息
        case Message:
            out << userName << getMessage();
            ui->messageBrowser->verticalScrollBar()->setValue(ui->messageBrowser->verticalScrollBar()->maximum());
            qDebug()<<"send : Message";
            break;

        // 新玩家
        case NewParticipant:
            out << localHostName <<userName<< address;
            break;

        case Gamesite:
            out << userName << getGame();
            qDebug()<<"send : Gamesite";
            break;

        case DOWN:
            out << userName << isdown;
            qDebug()<<"send : DOWN";
            break;

        case ParticipantLeft:
            out << userName;
            qDebug()<<"send : ParticipantLeft";
            break;

        case Ready:
            out << userName;
            qDebug()<<"send : Ready";
            break;

        case GiveUp:
            qDebug()<<"send : GiveUp";
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,targetport);
}

QString onlinePKGame::getMessage()
{
    QString msg = ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

QPoint onlinePKGame::getGame()
{
    return QPoint(clickPosRow,clickPosCol);
}
QString onlinePKGame::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
       return 0;
}
QString onlinePKGame::getUserName()
{
    QStringList stringlist;

    stringlist << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, stringlist)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
                break;
            }
        }
    }
    return "unknown";
}

void onlinePKGame::getUser(QString uid,QString user,int Type)
{
    uID = uid;
    userName = user;
    type = Type;
}

void onlinePKGame::participantLeft(QString userName, QString time)
{
    int rowNum = ui->userTableWidget->findItems(userName, Qt::MatchExactly).first()->row();
    ui->userTableWidget->removeRow(rowNum);
    ui->messageBrowser->setTextColor(Qt::black);
    ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
    ui->messageBrowser->append(QStringLiteral("%1离开房间%2").arg(userName).arg(time));
}
void onlinePKGame::newParticipant(QString localHostName,QString userName, QString ipAddress)
{
    bool isEmpty = ui->userTableWidget->findItems(userName,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
        ui->userTableWidget->verticalHeader()->setVisible(false);
        ui->userTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::Section{background:rgb(178, 113, 60);}");
        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,host);
        ui->userTableWidget->setItem(0,1,user);
        ui->userTableWidget->setItem(0,2,ip);
        ui->messageBrowser->setTextColor(Qt::black);
        ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->messageBrowser->append(QStringLiteral("%1在线").arg(userName));
        //sendMessage(NewParticipant);
    }
}

void onlinePKGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    // 绘制棋盘边界边框
    QPen pen;
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(20,40,kBoard,kBoard); //起始点：(20,40)
    //painter.drawRect(20,kBoardMargin - 20,size().width() - kBoardMargin,size().width() - kBoardMargin);

    // 绘制棋盘
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        //40+40*i,60     ,40+40*i,620
        //40     ,60+40*i,600    ,60+40*i
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin + 20, kBoardMargin + kBlockSize * i, kBoardHeight - kBoardMargin);
        painter.drawLine(kBoardMargin, kBoardMargin + 20 + kBlockSize * i, kBoardHeight - kBoardMargin - 20, kBoardMargin + 20 + kBlockSize * i);
    }

    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        if (game->chess_flag)
        {
            brush.setColor(Qt::white);
        }
        else
        {
            brush.setColor(Qt::black);
        }
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, 20 + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }
    //绘制棋盘的5个黑点
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawRect(155,175,10,10);
    painter.drawRect(475,175,10,10);
    painter.drawRect(155,495,10,10);
    painter.drawRect(475,495,10,10);
    painter.drawRect(315,335,10,10);

    // 绘制棋子
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (game->gameMapVec[i][j] == 1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, 20 + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
            else if (game->gameMapVec[i][j] == -1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, 20 + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
            }
        }
    }
    //绘制棋子示例
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.drawEllipse(686,40,50,50);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawEllipse(776,40,50,50);
    if(game->chess_flag)
    {
       QPoint points[3]={QPoint(700,120),QPoint(710,105),QPoint(720,120)};
       painter.drawPolygon(points,3);
    }
    else
    {
        QPoint points[3]={QPoint(790,120),QPoint(800,105),QPoint(810,120)};
        painter.drawPolygon(points,3);
    }
}
void onlinePKGame::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能落子
    if (x >= (kBoardMargin     + kBlockSize   / 2 ) &&
        x <  (kBoardHeight     - kBoardMargin - 50)&&
        y >= (kBoardMargin+30  + kBlockSize   / 2 ) &&
        y <  (kBoardHeight     - kBoardMargin     ))
    {
        clickPosRow = (event->y() - 10) / 40;
        clickPosCol = (event->x() - 15) / 40;
    }

    update();
}
void onlinePKGame::mouseReleaseEvent(QMouseEvent *event)
{
    // 玩家模式
    if (!(game_mode == COMPUTER && !game->chess_flag))
    {
        if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
        {
            if(isready && step_flg && !isdown) //isready = true,isdown = false
            {
                step_flg = false;
                // 重绘
                //QSound::play(CHESS_ONE_SOUND);
                game->PlayWithPerson(clickPosRow,clickPosCol);
                update();
                QMessageBox::StandardButton button;
                if(game->isWin(clickPosRow,clickPosCol))
                {
                    game->playType = WIN;
                    if(game->gameMapVec[clickPosRow][clickPosCol] == 1)
                    {
                        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
                    }
                    else
                    {
                        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
                    }
                }
                // 重置游戏状态
                if (button == QMessageBox::Ok)
                {
                    game->start(game_mode);
                    game->playType = PLAYING;

                    // 按钮
                    ui->readyButton->setEnabled(true);
                    // 标记初始化
                    myReady = 0;
                    otherReady = 0;
                    isready = false;
                    isdown = false;
                    step_flg = false;
                }

                //发送下棋信息
                sendMessage(Gamesite);
                isdown = true;
                sendMessage(DOWN);
            }

        }
    }
}

void onlinePKGame::on_playAction_triggered()
{
    sendMessage(ParticipantLeft);

    sql->update_type(uID,0);
    sql->reset_play(uID);
    sql->close_mysql();

    this->close();

    MainWindow *mainWin = new MainWindow;
    mainWin->show();
}

void onlinePKGame::on_computerAction_triggered()
{
    sendMessage(ParticipantLeft);

    sql->update_type(uID,0);
    sql->reset_play(uID);
    sql->close_mysql();

    this->close();

    ComputerPKGame *computerWin = new ComputerPKGame;
    computerWin->show(); 
    int ret = computerWin->messagebox();
    if(ret == QMessageBox::Yes)
    {
        computerWin->userFirst();
    }
    else if(ret==QMessageBox::No)
    {
        computerWin->computerFirst();
    }
    game->start(game_mode);
    game->playType = PLAYING;
}


void onlinePKGame::on_saveAction_triggered()
{
    QPixmap pixmap = this->grab(QRect(10,30,620,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"../my_Gomoku/", "Image File(*.jpg)");
    if(filename.length()>0)
    {
        pixmap.save(filename);
    }
}
void onlinePKGame::on_readyButton_clicked()
{
// 是玩家先准备好了
    if(otherReady == 0)
    {
        myReady = 1;
    }
// 是对手先准备好了
    else
    {
        myReady = 2;
    }

    ui->messageBrowser->setTextColor(Qt::black);
    ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->messageBrowser->append(QStringLiteral("%1准备").arg(userName));

    qDebug()<<"myReady = "<<myReady;
    qDebug()<<"otherReady = "<<otherReady;

    if(myReady != 0&& otherReady != 0)
    {
        if(myReady < otherReady)
        {
//game->chess_flag = true;//白棋
            step_flg = true;
        }
        qDebug()<<"step_flg = ture";
//else
//{
    //game->chess_flag = false;//黑棋
//}
        update();
        ui->messageBrowser->append(QStringLiteral("游戏开始!"));
        isready = true;
        qDebug()<< "游戏开始!";
    }

    sendMessage(Ready);
    //按钮不可选
    ui->readyButton->setEnabled(false);
}

void onlinePKGame::on_failButton_clicked()
{
    QMessageBox::StandardButton button;
    if(isready)
    {
        int ret=QMessageBox::question(this,QStringLiteral("认输"),QStringLiteral("确定认输?"),QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            if(otherReady == 1)
            {
                button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("白棋认输,黑棋获胜"));
            }
            else
            {
                button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("黑棋认输,白棋获胜"));
            }
            // 重置游戏状态
            if (button == QMessageBox::Ok)
            {
                game->start(game_mode);
                game->playType = PLAYING;
            }

            sendMessage(GiveUp);
        }
    }
    else
    {
        QMessageBox::information(this,QStringLiteral("警告"),QStringLiteral("游戏还未开始"));
    }
}

void onlinePKGame::on_sendButton_clicked()
{
    if(ui->messageTextEdit->toPlainText() == "")
    {
        QMessageBox::warning(0,"警告",QStringLiteral("文本不能为空"),QMessageBox::Ok);
        return;
    }
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->messageBrowser->setTextColor(Qt::blue);
    ui->messageBrowser->setCurrentFont(QFont("Times New Roman",11));
    ui->messageBrowser->append("["+userName+"]"+time);
    ui->messageBrowser->append(ui->messageTextEdit->toHtml());

    sendMessage(Message);
}
void onlinePKGame::on_backAction_triggered()
{
    sendMessage(ParticipantLeft);

    sql->update_type(uID,0);
    sql->reset_play(uID);
    sql->close_mysql();

    this->close();

    HomeWindow *homeWin = new HomeWindow;
    homeWin->show();
}

void onlinePKGame::on_exitAction_triggered()
{
    sendMessage(ParticipantLeft);

    sql->update_type(uID,0);
    sql->reset_play(uID);
    sql->close_mysql();

    this->close();
}
