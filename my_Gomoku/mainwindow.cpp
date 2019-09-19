#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "computerpkgame.h"
#include "homewindow.h"
#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <math.h>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QFileDialog>

/********* 全局属性  **********/
const int kBoard = 600;
const int kBoardWidth = 850;
const int kBoardHeight = 660;
const int kBoardMargin = 40; // 棋盘边缘空隙30
const int kRadius = 15;      // 棋子半径
const int kMarkSize = 6;     // 落子标记边长
const int kBlockSize = 40;   // 格子的大小
const int kPosDelta = 20;    // 鼠标点击的模糊距离上限
const int kAIDelay = 700;    // AI下棋的思考时间
/*****************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_玩家PK");

    //设置面板颜色
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);

    // 设置棋盘大小
    this->setFixedSize(kBoardWidth,kBoardHeight);
    this->setAutoFillBackground(true);

    //开启鼠标hover功能
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

    //初始化
    whiteTimes = 300;//300
    blackTimes = 300;
    btimerId = new QTimer(this);
    wtimerId = new QTimer(this);
    connect(btimerId,SIGNAL(timeout()),this,SLOT(btimerUpdate()));
    connect(wtimerId,SIGNAL(timeout()),this,SLOT(wtimerUpdate()));

    // 开始游戏
    initGame();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (game)
    {
        delete game;
        game = nullptr;
    }

}
void MainWindow::initGame()
{
    // 初始化游戏模型
    game = new GomokuModel;
    initPVPGame();
}

void MainWindow::initPVPGame()
{
    game_mode = PERSON;
    game->playType = PLAYING;
    game->start(game_mode);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
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

    //棋子倒计时示例
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawEllipse(700,120,50,50);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.drawEllipse(700,400,50,50);

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
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

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(game->chess_flag)
    {
        wtimerId->stop();
        btimerId->start(1000);
    }
    else
    {
        btimerId->stop();
        wtimerId->start(1000);
    }
    if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        game->PlayWithPerson(clickPosRow, clickPosCol);
        //QSound::play(CHESS_ONE_SOUND);
        // 重绘
        update();
    }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
       (game->gameMapVec[clickPosRow][clickPosCol] != 0))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->playType == PLAYING)
        {
            game->playType = WIN;
            wtimerId->stop();
            btimerId->stop();
            //QSound::play(WIN_SOUND);
            QMessageBox::StandardButton button;
            if (game->gameMapVec[clickPosRow][clickPosCol] == 1)
            {
                button =QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
            }
            else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
            {
                button =QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
            }
            // 重置游戏状态
            if (button == QMessageBox::Ok)
            {
                game->start(game_mode);
                game->playType = PLAYING;
            }
        }
    }

    // 判断死局
    if (game->isDead())
    {
        //QSound::play(LOSE_SOUND);
        QMessageBox::StandardButton button = QMessageBox::information(this, "警告", "此盘为死局!请重新再来！");
        if (button == QMessageBox::Ok)
        {
            game->start(game_mode);
            game->playType = PLAYING;
        }
    }
}

//void MainWindow::chess_person()
//{
//    // 根据当前存储的坐标下子
//    // 只有有效点击才下子，并且该处没有子
//    if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
//    {
//        game->PlayWithPerson(clickPosRow, clickPosCol);
//        //QSound::play(CHESS_ONE_SOUND);

//        // 重绘
//        update();
//    }
//}

void MainWindow::btimerUpdate()
{
    QString timeString=QString("%1-%2").arg(whiteTimes/60,2,10,QChar('0')).arg(whiteTimes%60,2,10,QChar('0'));
    ui->whiteTime->setText(timeString);
    QMessageBox::StandardButton button;
    if(!whiteTimes)
    {
        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("白棋超时,黑棋获胜!"));
        // 重置游戏状态
        if (button == QMessageBox::Ok)
        {
            game->start(game_mode);
            game->playType = PLAYING;
            whiteTimes = 300;
            blackTimes = 300;
        }
        return;
    }
    else
    {
        whiteTimes--;
    }
}

void MainWindow::wtimerUpdate()
{
    QString timeString=QString("%1-%2").arg(blackTimes/60,2,10,QChar('0')).arg(blackTimes%60,2,10,QChar('0'));
    ui->blackTime->setText(timeString);
    QMessageBox::StandardButton button;
    if(!blackTimes)
    {
        button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("黑棋超时,白棋获胜!"));
        // 重置游戏状态
        if (button == QMessageBox::Ok)
        {
            game->start(game_mode);
            game->playType = PLAYING;
            whiteTimes = 300;
            blackTimes = 300;
        }
        return;
    }
    else
    {
        blackTimes--;
    }
}

void MainWindow::on_playerPKAction_triggered()
{
    initPVPGame();
}


void MainWindow::on_computerPKAction_triggered()
{
    this->close();
    int ret;
    ComputerPKGame *computerWin = new ComputerPKGame;
    computerWin->show();
    ret = computerWin->messagebox();
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


void MainWindow::on_BackAction_triggered()
{
    this->close();
    HomeWindow* homeWin=new HomeWindow;
    homeWin->show();
}

void MainWindow::on_SaveAction_triggered()
{
    QPixmap pixmap=this->grab(QRect(10,30,620,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"../my_Gomoku/", "Image File(*.jpg)");
    if(filename.length()>0)
    {
        pixmap.save(filename);
    }
}
