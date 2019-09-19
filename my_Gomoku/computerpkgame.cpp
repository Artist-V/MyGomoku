#include "computerpkgame.h"
#include "ui_computerpkgame.h"
#include "homewindow.h"
#include "mainwindow.h"
#include "onlinepkgame.h"
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

/********* 全局属性  ********************************/
const int kBoard = 600;
const int kBoardWidth = 660;
const int kBoardHeight = 660;
const int kBoardMargin = 40; // 棋盘边缘空隙30
const int kRadius = 15;      // 棋子半径
const int kMarkSize = 6;     // 落子标记边长
const int kBlockSize = 40;   // 格子的大小
const int kPosDelta = 20;    // 鼠标点击的模糊距离上限
const int kAIDelay = 850;    // AI下棋的思考时间
/**************************************************/

ComputerPKGame::ComputerPKGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComputerPKGame)
{
    ui->setupUi(this);
    this->setWindowTitle("谢昕贝的五子棋_人机PK");

    //设置面板颜色
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);

    // 设置棋盘大小
    this->setFixedSize(kBoardWidth,kBoardHeight);
    this->setAutoFillBackground(true);

    //开启鼠标hover功能
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);

    // 开始游戏
    initGame();
}

ComputerPKGame::~ComputerPKGame()
{
    delete ui;
    if (game)
    {
        delete game;
        game = nullptr;
    }

}

int ComputerPKGame::messagebox()
{
    QMessageBox box(QMessageBox::Question,QStringLiteral("选择"),QStringLiteral("由谁先手"));
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    box.setButtonText (QMessageBox::Yes,QString(QStringLiteral("电脑后手")));
    box.setButtonText (QMessageBox::No,QString(QStringLiteral("电脑先手")));
    return box.exec();
}

void ComputerPKGame::userFirst()
{
    game->chess_flag = true;
}

void ComputerPKGame::computerFirst()
{
    game->chess_flag = false;
    chess_computer();
}

void ComputerPKGame::initGame()
{
    // 初始化游戏模型
    game = new GomokuModel;
    initPVCGame();
}

void ComputerPKGame::initPVCGame()
{
    game_mode = COMPUTER;
    game->playType = PLAYING;
    game->start(game_mode);
    update();
}

void ComputerPKGame::paintEvent(QPaintEvent *event)
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

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
       (game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
        game->gameMapVec[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->playType == PLAYING)
        {
            game->playType = WIN;
            //QSound::play(WIN_SOUND);
            QMessageBox::StandardButton button;
            if (game->gameMapVec[clickPosRow][clickPosCol] == 1)
            {
                button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("白棋获胜"));
            }
            else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
            {
                button = QMessageBox::information(this,QStringLiteral("恭喜"),QStringLiteral("黑棋获胜"));
            }
            // 重置游戏状态
            if (button == QMessageBox::Ok)
            {
                game->start(game_mode);
                game->playType = PLAYING;
            }

            int ret = messagebox();
            if(ret == QMessageBox::Yes)
            {
                userFirst();
            }
            else if(ret==QMessageBox::No)
            {
                computerFirst();
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

void ComputerPKGame::mouseMoveEvent(QMouseEvent *event)
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

void ComputerPKGame::mouseReleaseEvent(QMouseEvent *event)
{
    // 玩家下棋
    if (!(game_mode == COMPUTER && !game->chess_flag))
    {
        chess_person();

        // 人机PK
        if (game->gameMode == COMPUTER && !game->chess_flag)
        {
            // 延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chess_computer()));
        }
    }
}

void ComputerPKGame::chess_person()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        game->PlayWithPerson(clickPosRow, clickPosCol);
        //QSound::play(CHESS_ONE_SOUND);

        // 重绘
        update();
    }
}

void ComputerPKGame::chess_computer()
{
    game->PlayWithComputer(clickPosRow, clickPosCol);
    //QSound::play(CHESS_ONE_SOUND);
    update();
}


void ComputerPKGame::on_backAction_triggered()
{
    this->close();
    HomeWindow *homeWin = new HomeWindow;
    homeWin->show();
}

void ComputerPKGame::on_saveAction_triggered()
{
    QPixmap pixmap=this->grab(QRect(10,30,620,620));
    QString filename = QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"../my_Gomoku/", "Image File(*.jpg)");
    if(filename.length()>0)
    {
        pixmap.save(filename);
    }
}

void ComputerPKGame::on_playPKAction_triggered()
{
    this->close();
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
}

void ComputerPKGame::on_computerPKAction_triggered()
{
    int ret = messagebox();
    if(ret == QMessageBox::Yes)
    {
       userFirst();
    }
    else if(ret==QMessageBox::No)
    {
       computerFirst();
    }
    game->start(game_mode);
    game->playType = PLAYING;
    //initPVCGame();
}
