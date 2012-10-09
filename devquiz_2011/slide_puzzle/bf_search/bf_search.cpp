/* bf_search.cpp */

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
string bf_search(int width, int height, string board)
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
    HASH<string, unsigned int> hs;  // ハッシュ

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
    hs.addPair(start->board);

    count = 0;
    // 幅優先探索
    while(!bfs_queue.empty()) {
        //count++;
        // 計算手順が10万を超えてもゴールに辿り着けなかったら諦める
        //if(count > 100000)
        //    break;

        // 先頭からの局面状態の取り出し
        STATE a = bfs_queue.front();
        bfs_queue.pop();

        // 次の手で可能な全ての局面に対して
        for(int i = 0; (n = adjacent[a->space][i]) != -1; i++) {
            b = a->board;

            // スワップの対象が壁でない場合
            if(a->block[n] != '=') {
                // スワップ
                b[a->space] = b[n];
                b[n] = '0';

                STATE c;

                // 既に登録された局面の場合
                if(hs.find(b) != NULL) {
                    continue;
                }
                // 登録されていない新しい局面の場合
                else {
                    c = new struct State;

                    c->board = b;
                    c->space = n;
                    c->block = a->block;
                    c->prev_state = a;

                    // 解を発見した場合
                    if(b == goal->board) {
                        // 検索結果を出力
                        output(result, c, width, height);

                        // メモリ領域の解放
                        for(int i = 0; i < size; i++) {
                            delete[] adjacent[i];
                        }
                        delete[] adjacent;
                        delete start;
                        delete goal;
                        delete c;

                        // 最初に見つけた結果が最短経路
                        return result;
                    }
                    else {
                        // 局面状態の末尾への追加
                        bfs_queue.push(c);
                        hs.addPair(c->board);
                    }
                }
            }// if
        }// for
    }// while

    // メモリ領域の解放
    for(int i = 0; i < size; i++) {
        delete[] adjacent[i];
    }
    delete[] adjacent;
    delete start;
    delete goal;

    // 解が見つけられなかった場合
    return result;
}
