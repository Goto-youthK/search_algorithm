#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <stack>
#include "hash.hpp"

// 探索方向を示すフラグ
const unsigned int FORWARD = 1; // スタートから生成した局面
const unsigned int BACKWARD = 0; // ゴールから生成した局面

// 継続局面の全生成の有無を示すフラグ
const unsigned int OPEN = 1;    // ある局面で未生成の継続局面が残っている
const unsigned int CLOSE = 0;   // ある局面で継続局面が全て生成されている

using namespace std;

// 型宣言
typedef struct State* STATE;
typedef pair<string, STATE> Options;
typedef pair<int, Options> Cost_Attribute;

// 局面状態の構造体
struct State {
    string board;               // 局面
    int space;                  // 空白位置
    string block;               // 壁配列
    STATE prev_state;           // 前の局面状態
    int direction;              // 探索方向フラグ
    int cost;                   // コスト
    int depth;                  // 深さ
    int kind;                   // 局面の開閉
};

/* 隣接行列 */
// 3*3の場合
static const char adjacent33[9][5] = {
    1, 3,-1,-1,-1,
    0, 2, 4,-1,-1,
    1, 5,-1,-1,-1,
    0, 4, 6,-1,-1,
    1, 3, 5, 7,-1,
    2, 4, 8,-1,-1,
    3, 7,-1,-1,-1,
    4, 6, 8,-1,-1,
    5, 7,-1,-1,-1,
};

// 3*4の場合
static const char adjacent34[12][5] = {
    1, 4,-1,-1,-1,
    0, 2, 5,-1,-1,
    1, 3, 6,-1,-1,
    2, 7,-1,-1,-1,
    0, 5, 8,-1,-1,
    1, 4, 6, 9,-1,
    2, 5, 7,10,-1,
    3, 6,11,-1,-1,
    4, 9,-1,-1,-1,
    5, 8,10,-1,-1,
    6, 9,11,-1,-1,
    7,10,-1,-1,-1,
};

// 4*3の場合
static const char adjacent43[12][5] = {
    1, 3,-1,-1,-1,
    0, 2, 4,-1,-1,
    1, 5,-1,-1,-1,
    0, 4, 6,-1,-1,
    1, 3, 5, 7,-1,
    2, 4, 8,-1,-1,
    3, 7, 9,-1,-1,
    4, 6, 8,10,-1,
    5, 7,11,-1,-1,
    6,10,-1,-1,-1,
    7, 9,11,-1,-1,
    8,10,-1,-1,-1,
};

// 3*5の場合
static const char adjacent35[15][5] = {
    1, 5,-1,-1,-1,
    0, 2, 6,-1,-1,
    1, 3, 7,-1,-1,
    2, 4, 8,-1,-1,
    3, 9,-1,-1,-1,
    0, 6,10,-1,-1,
    1, 5, 7,11,-1,
    2, 6, 8,12,-1,
    3, 7, 9,13,-1,
    4, 8,14,-1,-1,
    5,11,-1,-1,-1,
    6,10,12,-1,-1,
    7,11,13,-1,-1,
    8,12,14,-1,-1,
    9,13,-1,-1,-1,
};

// 5*3の場合
static const char adjacent53[15][5] = {
    1, 3,-1,-1,-1,
    0, 2, 4,-1,-1,
    1, 5,-1,-1,-1,
    0, 4, 6,-1,-1,
    1, 3, 5, 7,-1,
    2, 4, 8,-1,-1,
    3, 7, 9,-1,-1,
    4, 6, 8,10,-1,
    5, 7,11,-1,-1,
    6,10,12,-1,-1,
    7, 9,11,13,-1,
    8,10,14,-1,-1,
    9,13,-1,-1,-1,
    10,12,14,-1,-1,
    11,13,-1,-1,-1,
};

// 4*4の場合
static const char adjacent44[16][5] = {
    1, 4,-1,-1,-1,
    0, 2, 5,-1,-1,
    1, 3, 6,-1,-1,
    2, 7,-1,-1,-1,
    0, 5, 8,-1,-1,
    1, 4, 6, 9,-1,
    2, 5, 7,10,-1,
    3, 6,11,-1,-1,
    4, 9,12,-1,-1,
    5, 8,10,13,-1,
    6, 9,11,14,-1,
    7,10,15,-1,-1,
    8,13,-1,-1,-1,
    9,12,14,-1,-1,
    10,13,15,-1,-1,
    11,14,-1,-1,-1,
};

// 3*6の場合
static const char adjacent36[18][5] = {
    1, 6,-1,-1,-1,
    0, 2, 7,-1,-1,
    1, 3, 8,-1,-1,
    2, 4, 9,-1,-1,
    3, 5,10,-1,-1,
    4,11,-1,-1,-1,
    0, 7,12,-1,-1,
    1, 6, 8,13,-1,
    2, 7, 9,14,-1,
    3, 8,10,15,-1,
    4, 9,11,16,-1,
    5,10,17,-1,-1,
    6,13,-1,-1,-1,
    7,12,14,-1,-1,
    8,13,15,-1,-1,
    9,14,16,-1,-1,
    10,15,17,-1,-1,
    11,16,-1,-1,-1,
};

// 6*3の場合
static const char adjacent63[18][5] = {
    1, 3,-1,-1,-1,
    0, 2, 4,-1,-1,
    1, 5,-1,-1,-1,
    0, 4, 6,-1,-1,
    1, 3, 5, 7,-1,
    2, 4, 8,-1,-1,
    3, 7, 9,-1,-1,
    4, 6, 8,10,-1,
    5, 7,11,-1,-1,
    6,10,12,-1,-1,
    7, 9,11,13,-1,
    8,10,14,-1,-1,
    9,13,15,-1,-1,
    10,12,14,16,-1,
    11,13,17,-1,-1,
    12,16,-1,-1,-1,
    13,15,17,-1,-1,
    14,16,-1,-1,-1,
};

// 4*5の場合
static const char adjacent45[20][5] = {
    1, 5,-1,-1,-1,
    0, 2, 6,-1,-1,
    1, 3, 7,-1,-1,
    2, 4, 8,-1,-1,
    3, 9,-1,-1,-1,
    0, 6,10,-1,-1,
    1, 5, 7,11,-1,
    2, 6, 8,12,-1,
    3, 7, 9,13,-1,
    4, 8,14,-1,-1,
    5,11,15,-1,-1,
    6,10,12,16,-1,
    7,11,13,17,-1,
    8,12,14,18,-1,
    9,13,19,-1,-1,
    10,16,-1,-1,-1,
    11,15,17,-1,-1,
    12,16,18,-1,-1,
    13,17,19,-1,-1,
    14,18,-1,-1,-1,
};

// 5*4の場合
static const char adjacent54[20][5] = {
    1, 4,-1,-1,-1,
    0, 2, 5,-1,-1,
    1, 3, 6,-1,-1,
    2, 7,-1,-1,-1,
    0, 5, 8,-1,-1,
    1, 4, 6, 9,-1,
    2, 5, 7,10,-1,
    3, 6,11,-1,-1,
    4, 9,12,-1,-1,
    5, 8,10,13,-1,
    6, 9,11,14,-1,
    7,10,15,-1,-1,
    8,13,16,-1,-1,
    9,12,14,17,-1,
    10,13,15,18,-1,
    11,14,19,-1,-1,
    12,17,-1,-1,-1,
    13,16,18,-1,-1,
    14,17,19,-1,-1,
    15,18,-1,-1,-1,
};

static const char adjacent46[24][5] = {
    1, 6,-1,-1,-1,
    0, 2, 7,-1,-1,
    1, 3, 8,-1,-1,
    2, 4, 9,-1,-1,
    3, 5,10,-1,-1,
    4,11,-1,-1,-1,
    0, 7,12,-1,-1,
    1, 6, 8,13,-1,
    2, 7, 9,14,-1,
    3, 8,10,15,-1,
    4, 9,11,16,-1,
    5,10,17,-1,-1,
    6,13,18,-1,-1,
    7,12,14,19,-1,
    8,13,15,20,-1,
    9,14,16,21,-1,
    10,15,17,22,-1,
    11,16,23,-1,-1,
    12,19,-1,-1,-1,
    13,18,20,-1,-1,
    14,19,21,-1,-1,
    15,20,22,-1,-1,
    16,21,23,-1,-1,
    17,22,-1,-1,-1,
};

static const char adjacent64[24][5] = {
    1, 4,-1,-1,-1,
    0, 2, 5,-1,-1,
    1, 3, 6,-1,-1,
    2, 7,-1,-1,-1,
    0, 5, 8,-1,-1,
    1, 4, 6, 9,-1,
    2, 5, 7,10,-1,
    3, 6,11,-1,-1,
    4, 9,12,-1,-1,
    5, 8,10,13,-1,
    6, 9,11,14,-1,
    7,10,15,-1,-1,
    8,13,16,-1,-1,
    9,12,14,17,-1,
    10,13,15,18,-1,
    11,14,19,-1,-1,
    12,17,20,-1,-1,
    13,16,18,21,-1,
    14,17,19,22,-1,
    15,18,23,-1,-1,
    16,21,-1,-1,-1,
    17,20,22,-1,-1,
    18,21,23,-1,-1,
    19,22,-1,-1,-1,
};

// 5*5の場合
static const char adjacent55[25][5] = {
    1, 5,-1,-1,-1,
    0, 2, 6,-1,-1,
    1, 3, 7,-1,-1,
    2, 4, 8,-1,-1,
    3, 9,-1,-1,-1,
    0, 6,10,-1,-1,
    1, 5, 7,11,-1,
    2, 6, 8,12,-1,
    3, 7, 9,13,-1,
    4, 8,14,-1,-1,
    5,11,15,-1,-1,
    6,10,12,16,-1,
    7,11,13,17,-1,
    8,12,14,18,-1,
    9,13,19,-1,-1,
    10,16,20,-1,-1,
    11,15,17,21,-1,
    12,16,18,22,-1,
    13,17,19,23,-1,
    14,18,24,-1,-1,
    15,21,-1,-1,-1,
    16,20,22,-1,-1,
    17,21,23,-1,-1,
    18,22,24,-1,-1,
    19,23,-1,-1,-1,
};
//static const char adjacent56[30][5] = {};
//static const char adjacent65[30][5] = {};
//static const char adjacent66[36][5] = {};

int board_width;                // 局面の幅
int board_height;               // 局面の高さ

// 転倒距離
int invert_distance(string board)
{
    int size = board.size();
    int tentou;                 // 転倒数(数字の並び順が逆になっている数)
    int req_ud;                 // 最低限必要な上下動回数
    int req_lr;                 // 最低限必要な左右動回数
    int ID;                     // 転倒距離

    // 転倒数の計算
    for(int i = 0; i < size; i++) {
        if(board[i] != '=') {
            for(int j = 1; i+j < size; j++) {
                if(board[i+j] != '=' && board[i] > board[i+j]) {
                    tentou++;
                }
            }
        }
    }

    // 転倒距離の計算
    req_ud = (tentou / board_width) + (tentou % board_width);
    req_lr = (tentou / board_height) + (tentou % board_height);
    ID = req_ud + req_lr;

    return ID;
}

void coordinate_transform(int panel_num, int& panel_x, int& panel_y)
{
    if(panel_num <= board_width) {
        panel_x = panel_num;
    }
    else {
        if(panel_num % board_width == 0)
            panel_x = board_width;
        else
            panel_x = panel_num % board_width;
    }

    if(panel_num % board_width == 0)
        panel_y = (int)(panel_num / board_width);
    else
        panel_y = (int)(panel_num / board_width) + 1;
}

// マンハッタン距離
int manhattan_distance(char from_panel, int position, string to_board)
{
    int size = to_board.size();
    char to_panel;                  // パネル
    int from_panel_x, from_panel_y; // 現在位置
    int to_panel_x, to_panel_y;     // 目標位置
    int MD;                         // マンハッタン距離

    MD = 0;
    for(int i = 1; i <= size; i++) {
        to_panel = to_board[i-1];

        if((int)from_panel == (int)to_panel) {
            // positionから現在位置のx,y座標に変換
            coordinate_transform(position, from_panel_x, from_panel_y);
            // iから目標位置のx,y座標に変換
            coordinate_transform(i, to_panel_x, to_panel_y);

            // マンハッタン距離の計算
            MD += abs(from_panel_x - to_panel_x) + abs(from_panel_y - to_panel_y);
        }
    }

    return MD;
}

// 盤面からコスト(移動距離)を計算
int get_distance(string from_board, string start_board, string goal_board, int direction)
{
    int size = from_board.size();
    int total;                  //
    char from_panel;            // パネル

    total = 0;
    for(int i = 1; i <= size; i++) {
        from_panel = from_board[i-1];

        if(from_panel == '0' || from_panel == '=')
            continue;
        else {
            if(direction == FORWARD)
                total += manhattan_distance(from_panel, i, goal_board);
            else
                total += manhattan_distance(from_panel, i, start_board);
        }
    }

    return total;
}

/*
// プレディケート
class LessInt {
public:
    bool operator()(const int& ri_left, const Options& ri_right) const {
        return ri_left < ri_right;
    }
};
*/

#endif
