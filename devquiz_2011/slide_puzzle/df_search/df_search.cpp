/* df_search.cpp */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
//#include <ext/hash_map>
#include <stack>
#include <list>
#include "search.hpp"
#include "hash.hpp"
#include "init.hpp"
#include "output.hpp"

using namespace std;
using namespace __gnu_cxx;

// プロトタイプ宣言
bool dfs(string& result, STATE state, STATE goal, stack<STATE> dfs_stack, HASH<string, unsigned int> hs, char** adjacent);

/* 探索 */
string df_search(int width, int height, string board)
{
    int size = width*height;        // パネル数
    char** adjacent;                // 隣接配列
    STATE start = new struct State; // スタートの局面状態
    STATE goal = new struct State;  // ゴールの局面状態
    stack<STATE> dfs_stack;         // スタック
    HASH<string, unsigned int> hs;  // ハッシュ
    string result = "";             // 検索結果

    // グローバル変数への値の代入
    board_width = width;
    board_height = height;

    // 隣接行列の領域の動的確保
    adjacent = new char*[size];
    for(int i = 0; i < size; i++)
        adjacent[i] = new char[5];

    // 該当する隣接行列への置換
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < 5; j++) {// U D L R
            switch(size) {
            case  9:
                adjacent[i][j] = adjacent33[i][j];
                break;
            case 12:
                if(width > height) adjacent[i][j] = adjacent34[i][j];
                else adjacent[i][j] = adjacent43[i][j];
                break;
            case 15:
                if(width > height) adjacent[i][j] = adjacent35[i][j];
                else adjacent[i][j] = adjacent53[i][j];
                break;
            case 16:
                adjacent[i][j] = adjacent44[i][j];
                break;
            case 18:
                if(width > height) adjacent[i][j] = adjacent36[i][j];
                else adjacent[i][j] = adjacent63[i][j];
                break;
            case 20:
                if(width > height) adjacent[i][j] = adjacent45[i][j];
                else adjacent[i][j] = adjacent54[i][j];
                break;
            case 24:
                if(width > height) adjacent[i][j] = adjacent46[i][j];
                else adjacent[i][j] = adjacent64[i][j];
                break;
            case 25:
                adjacent[i][j] = adjacent55[i][j];
                break;
            default:
                break;
            }
        }
    }

    // スタートとゴールの局面状態の設定
    init(board, start, goal);

    // スタートの局面状態の先頭への追加
    dfs_stack.push(start);
    hs.addPair(start->board);

    // 深さ優先探索
    dfs(result, start, goal, dfs_stack, hs, adjacent);

    // メモリ領域の解放
    for(int i = 0; i < size; i++) {
        delete[] adjacent[i];
    }
    delete[] adjacent;
    delete start;
    delete goal;

    return result;
}

// 深さ優先探索
bool dfs(string& result, STATE state, STATE goal, stack<STATE> dfs_stack, HASH<string, unsigned int> hs, char** adjacent)
{
    int n;                      // スワップ対象の位置
    string b = "";              // 局面状態の一次保存用
    char panel;                 // パネル
    int cost;                   // コスト
    int diff_ID;                // 転倒距離の差分
    int diff_MD;                // マンハッタン距離の差分
    list<Cost_Attribute>buff;
    list<Cost_Attribute>::iterator it;

    // 解を発見した場合
    if(state->board == goal->board) {
        // 検索結果を出力
        output(result, state, board_width, board_height);

        return true;
    }
    else {
        // 次の手で移動可能な全ての局面に対して
        for(int i = 0; (n = adjacent[state->space][i]) != -1; i++) {
            b = state->board;
            panel = b[n];

            // スワップの対象が壁でない場合
            if(state->block[n] != '=') {
                // スワップ
                b[state->space] = b[n];
                b[n] = '0';

                // 既に登録された局面の場合
                if(hs.find(b) != NULL) {
                    continue;
                }
                // 登録されていない新しい局面の場合
                else {
                    // 動かしたパネルの差分だけ計算
                    diff_ID = invert_distance(state->board) - invert_distance(b);
                    diff_MD = manhattan_distance(panel, state->space+1) - manhattan_distance(panel, n+1);

                    // より大きい(厳しい)値を採用する
                    //if(diff_MD > diff_ID)
                        cost = state->cost + diff_MD; // コストの更新
                        //else
                        //cost = state->cost + diff_ID; // コストの更新

                    // 移動可能な局面状態をリストに追加
                    buff.push_front(Cost_Attribute(cost, Options(b, state)));
                }
            }
        }

        // もしリストに追加されていた場合
        if(buff.size() > 0) {
            // コストの最も小さい局面から選択されるように並び替え
            buff.sort();

            for(it = buff.begin(); it != buff.end(); it++) {
                /*
                // クラスメンバへのポインタの定義
                int Cost_Attribute::*  mp_cost;
                Options Cost_Attribute::* mp_attribute;
                string Options::* mp_board;
                STATE Options::* mp_state;

                int candidate_cost = (*it).*mp_cost;
                string candidate_board = (*it).*mp_attribute.*mp_board;
                STATE candidate_state = (*it).*mp_attribute.*mp_state;
                string candidate_block = ((*it).*mp_attribute.*mp_state)->block;
                int candidate_space;
                */

                int candidate_cost = (*it).first;
                string candidate_board = (*it).second.first;
                STATE candidate_state = (*it).second.second;
                string candidate_block = (*candidate_state).block;
                int candidate_space;

                // 空白の位置
                for(int i = 0; i < candidate_board.size(); i++) {
                    if(candidate_board[i] == '0')
                        candidate_space = i;
                }

                STATE c = new struct State;

                c->board = candidate_board;
                c->space = candidate_space;
                c->block = candidate_block;
                c->prev_state = candidate_state;
                c->cost = candidate_cost;

                // 局面状態の先頭への追加
                dfs_stack.push(c);
                hs.addPair(c->board);

                // 再帰
                if(dfs(result, c, goal, dfs_stack, hs, adjacent)) {
                    delete c;

                    return true;
                }
                else
                    // 先頭からの局面状態の取り出し
                    dfs_stack.pop();

            }// for
        }// if
    }

    return false;
}
