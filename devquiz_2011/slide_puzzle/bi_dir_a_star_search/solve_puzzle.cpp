/* solve_puzzle.cpp */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>

using namespace std;

/* 探索関数 */
string bi_dir_a_star_search(int width, int height, string b); // 双方向A*探索

int main()
{
    int start_time, end_time;   // 開始時間，終了時間
    int LX, RX, UX, DX;         // 使用可能なL,R,U,Dそれぞれの総数
    int N;                      // 総問題数
    int width, height;          // 局面の幅(列)，高さ(行)
    int size;                   // サイズ
    char* chrBoard;             // 局面の最初の状態を表す文字列
    string strBoard;            // 局面の最初の状態を表す文字列
    string result;              // 解答(LRUD列)
    int count = 0;              // 解答数
    FILE *fp_in, *fp_out;       // 入出力ファイル用のポインタ

    fp_in  = fopen("infile.txt", "r");
    fp_out = fopen("outfile.txt", "w");

    fscanf(fp_in, "%d %d %d %d", &LX, &RX, &UX, &DX);
    fscanf(fp_in, "%d", &N);
    printf("LX = %d, RX = %d, UX = %d, DX = %d, N = %d \n", LX, RX, UX, DX, N);

    for(int i = 0; i < N; i++) {
        fscanf(fp_in, "%d, %d, %s", &width, &height, chrBoard = new char[width*height]);
        size = width*height;
        strBoard = chrBoard;

        printf("現在%d問目 \n", i+1);

        // 解答問題の制限
        if(size == 15) {
            start_time = clock();
            result = bi_dir_a_star_search(width, height, strBoard); // 探索
            end_time = clock();

            printf("解答(LRUD列) : %s \n", result.c_str());
            fprintf(fp_out, "%s\n", result.c_str());

            count++;

            printf("時間 : %lf 秒 \n", (double)((end_time - start_time)/CLOCKS_PER_SEC));
            printf("総解答数 : %d / 全5000問 \n", count);
        }
        else
            fprintf(fp_out, "\n");

        delete[] chrBoard;
    }

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}
