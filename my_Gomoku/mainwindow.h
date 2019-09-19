#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")
#include "gomokumodel.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

private:
    GomokuModel *game; // 游戏指针
    GameMode game_mode; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    int whiteTimes;//白棋时间
    int blackTimes;//黑棋时间
private:
    void initGame();
public:
    QTimer* wtimerId;
    QTimer* btimerId;
    void paintEvent(QPaintEvent *event);    // 绘制
    void mouseMoveEvent(QMouseEvent *event);// 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子
private slots:
    //void chess_person(); // 人执行
    void initPVPGame();
    void btimerUpdate();
    void wtimerUpdate();
    void on_computerPKAction_triggered();
    void on_playerPKAction_triggered();
    void on_BackAction_triggered();
    void on_SaveAction_triggered();
};

#endif // MAINWINDOW_H
