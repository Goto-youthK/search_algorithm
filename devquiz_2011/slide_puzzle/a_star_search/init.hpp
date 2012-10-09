#ifndef INIT_HPP
#define INIT_HPP

#include <cstdlib>
#include <cstring>
#include <string>
#include "search.hpp"

using namespace std;

// スタートとゴールの局面状態の設定
void init(string board, STATE start, STATE goal)
{
    int size = board.size();

    for(int i = 0; i < size; i++) {
        // 局面(スタート)
        start->board += board[i];

        // 局面(ゴール)
        if(board[i] != '=') {
            if(i != size-1) {
                if(i < 9)
                    goal->board += '0' + (i+1);   // 昇順の数字
                else
                    goal->board += 'A' + (i-9);   // 昇順の英字
            }
            else
                goal->board += '0'; // 配列の最後は空白
        }
        else
            goal->board += board[i]; // 壁
    }

    for(int i = 0; i < size; i++) {
        // 空白の位置(スタート)
        if(board[i] == '0')
            start->space = i;
    }
    // 空白の位置(ゴール)
    goal->space = size-1;

    for(int i = 0; i < size; i++) {
        // 壁の位置(スタート，ゴール)
        if(board[i] == '=') {
            start->block += '=';
            goal->block += '=';
        }
        else {
            start->block += ' ';
            goal->block += ' ';
        }
    }

    // 手前の局面(スタート，ゴール)
    start->prev_state = NULL;
    goal->prev_state = NULL;

    // コスト(スタート，ゴール)
    start->cost = get_distance(board);
    goal->cost = 0;

    // 深さ(スタート，ゴール)
    start->depth = 0;
    goal->depth = 0;

    // 局面の開閉(スタート，ゴール)
    start->kind = OPEN;
    goal->kind = OPEN;
}

#endif
