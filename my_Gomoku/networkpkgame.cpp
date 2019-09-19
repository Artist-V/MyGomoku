#include "networkpkgame.h"
#include "ui_networkpkgame.h"
#include "mainwindow.h"
#include "computerpkgame.h"
#include "homewindow.h"
#include "onlinepkgame.h"
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

networkPKGame::networkPKGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::networkPKGame)
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

    // 标记初始化
    firstReady = 0;
    myReady = 0;
    otherReady = 0;
    isready = false;
    isdown = false;

    //socket初始化
    initSocket();

    // 游戏初始化
    sendMessage(NewParticipant);
    initGame();
}

networkPKGame::~networkPKGame()
{
    delete ui;
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void networkPKGame::initSocket()
{
    udpSocket=new QUdpSocket(this);
    port = 9090;// 端口号
    // 绑定
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
}

void networkPKGame::initGame()
{
    game = new GomokuModel;
    game_mode = ONLINE;
    game->playType = PLAYING;
    game->start(game_mode);
    update();
}

void networkPKGame::processPendingDatagrams()
{
    // 有数据：
    // 如果至少有一个数据报等待读取，则返回true;否则返回false
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        // 返回数据报大小：
                            // 返回第一个挂起的UDP数据报的大小。如果没有可用的数据报，则此函数返回-1。
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in >> messageType;

        QString userName,ipAddress;
        //QString userName,localHostName,ipAddress,message;
        QPoint gamesite;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QMessageBox::StandardButton button;
        switch(messageType)
        {
            case Message:
                in >> userName >> ipAddress >> message;
                ui->messageBrowser->setTextColor(Qt::blue);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",11));
                ui->messageBrowser->append("["+userName+"]"+time);
                ui->messageBrowser->append(message);
                break;

            // 新玩家
            case NewParticipant:
                in >> userName >>  ipAddress;
                newParticipant(userName,ipAddress);
                break;

            // 玩家离开
            case ParticipantLeft:
                in >> userName;
                participantLeft(userName,time);
                break;

            // 认输
            case GiveUp:
                in >> userName ;
                if(game->chess_flag)
                {
                    button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("黑棋认输,白棋获胜"));
                }
                else
                {
                    button = QMessageBox::information(this,QStringLiteral("对战提醒"),QStringLiteral("白棋认输,黑棋获胜"));
                }
                // 重置游戏状态
                if (button == QMessageBox::Ok)
                {
                    game->start(game_mode);
                    game->playType = PLAYING;
                }
                break;

            // 游戏准备(先准备好的：白棋；后准备好的黑棋)
            case Ready:
                in >> userName ;
                if(localHostName == QHostInfo::localHostName())
                {
                    // 是玩家先准备好了
                    myReady = firstReady;
                }
                else
                {
                    // 是对手先准备好了
                    otherReady = firstReady;
                }
                firstReady ++;//置换标记

                ui->messageBrowser->setTextColor(Qt::black);
                ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->messageBrowser->append(QStringLiteral("%1准备").arg(userName));

                if(myReady != 0&& otherReady != 0)
                {
                    if(myReady < otherReady)
                    {
                        game->chess_flag = true;//白棋
                    }
                    else
                    {
                        game->chess_flag = false;//黑棋
                    }
                    update();
                    ui->messageBrowser->append(QStringLiteral("游戏开始！"));
                    isready = true;
                }
                break;

            // 游戏
            case Gamesite:
                in >> userName >> localHostName >> gamesite;
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
                // 重置游戏状态
                if (button == QMessageBox::Ok)
                {
                    game->start(game_mode);
                    game->playType = PLAYING;
                }
                break;

            case DOWN:
                in >>  localHostName;
                if(localHostName != QHostInfo::localHostName())
                {
                    isdown = true;
                }
                break;
        }
    }
}

void networkPKGame::sendMessage(MessageType type)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIP();
    out << type << getUserName() << localHostName;
    switch(type)
    {
        // 消息
        case Message:
            if(ui->messageTextEdit->toPlainText() == "")
            {
                QMessageBox::warning(0,"警告",QStringLiteral("文本不能为空"),QMessageBox::Ok);
                return;
            }
            out << address << getMessage();
            ui->messageBrowser->verticalScrollBar()->setValue(ui->messageBrowser->verticalScrollBar()->maximum());
            break;

        // 新玩家
        case NewParticipant:
            out << address;
            break;

        case Gamesite:
            out << getGame();
            break;

        case DOWN:
            out << isdown;
            break;

        case ParticipantLeft:
        case Ready:
        case GiveUp:
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

QString networkPKGame::getMessage()
{
    QString msg=ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

QPoint networkPKGame::getGame()
{
    return QPoint(clickPosRow,clickPosCol);
}
QString networkPKGame::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
       return 0;
}
QString networkPKGame::getUserName()
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


void networkPKGame::participantLeft(QString userName, QString time)
{
    int rowNum = ui->userTableWidget->findItems(userName, Qt::MatchExactly).first()->row();
    ui->userTableWidget->removeRow(rowNum);
    ui->messageBrowser->setTextColor(Qt::black);
    ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
    ui->messageBrowser->append(QStringLiteral("%1离开%2").arg(userName).arg(time));
}
void networkPKGame::newParticipant(QString userName, QString ipAddress)
{
    bool isEmpty = ui->userTableWidget->findItems(userName,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
        ui->userTableWidget->verticalHeader()->setVisible(false);
        ui->userTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::Section{background:rgb(178, 113, 60);}");
        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,2,ip);
        ui->messageBrowser->setTextColor(Qt::black);
        ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->messageBrowser->append(QStringLiteral("%1在线").arg(userName));
        sendMessage(NewParticipant);
    }
}

void networkPKGame::paintEvent(QPaintEvent *)
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
void networkPKGame::mouseMoveEvent(QMouseEvent *event)
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
void networkPKGame::mouseReleaseEvent(QMouseEvent *event)
{
    // 玩家模式
    if (!(game_mode == COMPUTER && !game->chess_flag))
    {
        if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
        {
            if(isready && !isdown)
            {
                sendMessage(Gamesite);
                isdown = true;
                sendMessage(DOWN);
            }
            //QSound::play(CHESS_ONE_SOUND);

            // 重绘
            update();
        }
    }
}

void networkPKGame::on_backAction_triggered()
{
    this->close();
    HomeWindow *homeWin = new HomeWindow;
    homeWin->show();
}

void networkPKGame::on_playerAction_triggered()
{
    this->close();
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
}

void networkPKGame::on_computerAction_triggered()
{
    this->close();
    ComputerPKGame *computerWin = new ComputerPKGame;
    computerWin->show();
}

void networkPKGame::on_onlineAction_triggered()
{
    this->close();
    onlinePKGame *onlineWin = new onlinePKGame;
    onlineWin->show();
}

void networkPKGame::on_saveAction_triggered()
{
    QPixmap pixmap=this->grab(QRect(10,30,620,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"./", "Image File(*.jpg)");
    if(filename.length()>0)
    {
        pixmap.save(filename);
    }
}

void networkPKGame::on_readyButton_clicked()
{
    sendMessage(Ready);
}

void networkPKGame::on_failButton_clicked()
{
    int ret=QMessageBox::question(this,QStringLiteral("认输"),QStringLiteral("确定认输?"),QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        sendMessage(GiveUp);

    }
}

void networkPKGame::on_sendButton_clicked()
{
    sendMessage(Message);
}
