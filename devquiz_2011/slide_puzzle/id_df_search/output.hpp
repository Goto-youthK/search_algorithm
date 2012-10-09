#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <cstdlib>
#include <cstring>
#include <string>
#include "search.hpp"

using namespace std;

void concat_lrud(string& result, int prev_space, int space, int width, int height)
{
    int diff = space - prev_space;

    if(diff == 1)
        result += 'R';          // 右に移動
    else if(diff == -1)
        result += 'L';          // 左に移動
    else if(diff == width)
        result += 'D';          // 下に移動
    else if(diff == -width)
        result += 'U';          // 上に移動
    else
        result += ' ';          // エラー

    cout << "result:" << result << endl;
}

void print_board(string board, int width, int height)
{
    int size = width*height;

    for(int i = 0; i < size; i++) {
        if(i%width == 0 && i != 0)
            printf("\n");
        printf("%d ", board[i] - '0');
    }
    printf("\n\n");
}

void output(string& result, STATE state, int width, int height)
{
    int prev_space;             // 前局面の空白位置
    int space;                  // 現局面の空白位置

    // スタート局面以外の場合
    if(state->prev_state != NULL) {
        STATE prev_state = new struct State;
        prev_state = state->prev_state;

        // 再帰
        output(result, prev_state, width, height);

        prev_space = prev_state->space;
        space = state->space;
        // LRUDの結合
        concat_lrud(result, prev_space, space, width, height);
    }

    // 局面状態の表示
    print_board(state->board, width, height);
}

#endif
