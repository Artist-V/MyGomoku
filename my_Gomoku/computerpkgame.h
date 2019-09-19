#ifndef COMPUTERPKGAME_H
#define COMPUTERPKGAME_H

#include "gomokumodel.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QVector>

namespace Ui {
class ComputerPKGame;
}

class ComputerPKGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComputerPKGame(QWidget *parent = 0);
    ~ComputerPKGame();
private:
    Ui::ComputerPKGame *ui;

private:
    GomokuModel *game; // 游戏指针
    GameMode game_mode; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置

private:
    void initGame();
public:
    void paintEvent(QPaintEvent *event);    // 绘制
    void mouseMoveEvent(QMouseEvent *event);// 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子
    int messagebox();
    void userFirst();
    void computerFirst();
private slots:
    void chess_person(); // 人执行
    void chess_computer(); // AI下棋
    void initPVCGame();
    void on_backAction_triggered();
    void on_saveAction_triggered();
    void on_playPKAction_triggered();
    void on_computerPKAction_triggered();
};

#endif // COMPUTERPKGAME_H
