#ifndef GOMOKUMODEL_H
#define GOMOKUMODEL_H
#pragma execution_character_set("utf-8")

#include <vector>

using namespace std;
const int kBoardSizeNum = 15 ;// 棋盘尺寸

// 游戏模式
enum GameMode
{
    PERSON,
    COMPUTER,
    ONLINE
};

// 游戏状态
enum PlayType
{
    PLAYING,
    WIN,
    DEAD
};



class GomokuModel
{
public:
    GomokuModel();

public:
    vector<vector<int>> gameMapVec; // 存储当前游戏棋子的情况,空白为0，白子1，黑子-1
    vector<vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为电脑下棋依据
    GameMode gameMode; // 游戏模式
    PlayType playType; // 游戏状态
    bool chess_flag; // 下棋方

    void start(GameMode mode); // 开始游戏
    void PlayWithPerson(int row, int col); // 玩家与玩家对战
    void calculateScore(); // 计算评分
    void PlayWithComputer(int &clickRow, int &clickCol); // 玩家与电脑对战
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋子信息
    bool isWin(int row, int col); // 判断游戏是否胜利
    bool isDead(); // 判断是否和棋

};

#endif // GOMOKUMODEL_H
