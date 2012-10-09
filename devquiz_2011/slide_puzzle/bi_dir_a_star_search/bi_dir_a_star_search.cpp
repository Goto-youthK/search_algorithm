/* bi_dir_a_star_search.cpp */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
//#include <ext/hash_map>
#include <queue>
//#include <list>
#include "search.hpp"
#include "hash.hpp"
#include "init.hpp"
#include "output.hpp"

using namespace std;
using namespace __gnu_cxx;

/* 探索 */
string bi_dir_a_star_search(int width, int height, string board)
{
    int size = width*height;        // パネル数
    char** adjacent;                // 隣接配列
    STATE start = new struct State; // スタートの局面状態
    STATE goal = new struct State;  // ゴールの局面状態
    int count;                      // 計算手順
    int n;                          // スワップ対象の位置
    string b = "";                  // 局面状態の一次保存用
    string result = "";             // 検索結果
    queue<STATE> bfs_queue;         // キュー
    HASH<string, STATE> hs;         // ハッシュ

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

    // スタートの局面状態の末尾への追加
    bfs_queue.push(start);
    hs.addPair(start);

    // ゴールの局面状態の末尾への追加
    bfs_queue.push(goal);
    hs.addPair(goal);

    count = 0;
    // 双方向A*探索
    while(!bfs_queue.empty()) {
        count++;
        // 計算手順が10万を超えてもゴールに辿り着けなかったら諦める
        if(count > 100000)
            return result;

        char panel;             // パネル
        int diff_MD;            // マンハッタン距離の差分

        // 先頭から局面状態の取り出し
        STATE a = bfs_queue.front();
        bfs_queue.pop();

        // 廃棄オブジェクトのチェック
        if(a->kind == CLOSE)
            continue;

        // 次の手で可能な全ての局面に対して
        for(int i = 0; (n = adjacent[a->space][i]) != -1; i++) {
            b = a->board;
            panel = b[n];

            // スワップの対象が壁でない場合
            if(a->block[n] != '=') {
                // スワップ
                b[a->space] = b[n];
                b[n] = '0';

                STATE c;

                // 動かしたパネルの差分だけ計算
                if(a->direction == FORWARD) {
                    diff_MD = manhattan_distance(panel, a->space+1, goal->board)
                        - manhattan_distance(panel, n+1, goal->board);
                }
                else {
                    diff_MD = manhattan_distance(panel, a->space+1, start->board)
                        - manhattan_distance(panel, n+1, start->board);
                }

                // 既に登録された局面の場合
                if(hs.find(b) != NULL) {
                    // 探索した局面状態の取り出し
                    c = hs.find(b);

                    // 双方向の探索で同一局面に到達した場合
                    if(c->direction != a->direction) {
                        // 検索結果を出力
                        output(result, a, c, width, height);

                        // メモリ領域の解放
                        for(int i = 0; i < size; i++) {
                            delete[] adjacent[i];
                        }
                        delete[] adjacent;
                        delete start;
                        delete goal;
                        delete c;

                        // 発見した最短経路
                        return result;
                    }

                    // コストは同じなので手数を比較して小さい場合
                    if(c->depth > a->depth + 1) {
                        // 局面状態の更新
                        if(c->kind == OPEN) {
                            c->board = b;
                            c->space = n;
                            c->block = a->block;
                            c->prev_state = a;
                            c->direction = a->direction;
                            c->cost = a->cost + 1 + diff_MD;
                            c->depth = a->depth + 1;
                            c->kind = CLOSE;

                            // 書き換え
                            hs.remove(c->board);
                            hs.addPair(c);
                        }
                        else {
                            c->prev_state = a;
                            c->cost = c->cost - c->depth + a->depth + 1;
                            c->depth = a->depth + 1;
                            c->kind = OPEN;
                        }

                        // 局面状態の末尾への追加
                        bfs_queue.push(c);
                    }
                }
                // 登録されていない新しい局面の場合
                else {
                    c = new struct State;

                    c->board = b;
                    c->space = n;
                    c->block = a->block;
                    c->prev_state = a;
                    c->direction = a->direction;
                    c->cost = a->cost + 1 + diff_MD;
                    c->depth = a->depth + 1;
                    c->kind = OPEN;

                    // 局面状態の末尾への追加
                    bfs_queue.push(c);
                    hs.addPair(c);
                }
            }// if
        }// for

        // a の継続局面は展開済み
        a->kind = CLOSE;
    }// while
}
