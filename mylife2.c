#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // sleep()関数を使う
//#include "gol.h"

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp);
void my_print_cells(FILE* fp, int gen, const int height, const int width, int cell[height][width]);
void my_update_cells(const int height, const int width, int cell[height][width]);
void my_update_file(int gen, const int height, const int width, int cell[height][width]);

int main(int argc, char **argv)
{
  //FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
      my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }
  else{
    my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
  }

    //my_print_cells(fp, 0, height, width, cell); // 表示する
    //sleep(1); // 1秒休止
    //fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)

  /* 世代を進める*/
  int cnt = 100;
  for (int gen = 1 ;gen<10000; gen++) {  
    my_update_cells(height, width, cell); // セルを更新
    //my_print_cells(fp, gen, height, width, cell);  // 表示する
    if(gen == cnt) {
        cnt += 100;
        my_update_file(gen, height, width, cell);
    }
    //sleep(1); //1秒休止する
    //fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}


void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp) {
    if (fp == NULL) {
        // cell[20][30] = 1;
        // cell[22][30] = 1;
        // cell[22][31] = 1;
        // cell[23][31] = 1;
        // cell[20][32] = 1;
        srand((unsigned)time(NULL));
        double a = 0;
        for (int i=0; i<1000; ++i) {
            a += (double)rand()/RAND_MAX;
        }
        double b = (a * 0.001 * 0.2);   //約0.1の乱数
        int c = b * height*width;
        for (int i=0; i<c; ++i) {
            while(1) {
                int k = rand()%height;
                int l = rand()%width;
                if(cell[k][l] == 0) {
                    cell[k][l] = 1;
                    break;
                }
            }
        }

    }
    else {
        int a, b;
        char bff[100];
        fgets(bff, 100, fp);
        while (fscanf(fp, "%d %d", &a, &b) != EOF) {
            cell[b][a] = 1;
        }
    }


}

// void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]) {
//     int cnt = 0;
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             if (cell[y][x] == 1) {
//                 ++cnt;
//             }
//         }
//     }
//     double ratio = 100 * (double)cnt/(height*width);
//     //上の壁
//     fprintf(fp,"generation = %d ratio = %.2f%%\n", gen, ratio); // この場合 (fp = stdout), printfと同じ
//     fprintf(fp,"+");
//     for (int x = 0 ; x < width ; x++) {
//         fprintf(fp, "-");
//     }
//     fprintf(fp, "+\n");
//     /* 外壁と 内側のゲーム部分 */
//     for (int y = 0; y < height; y++) {
//         fprintf(fp,"|");
//         for (int x = 0; x < width; x++) {
//             // ANSIエスケープコードを用いて、赤い"#" を表示
//             // \e[31m で 赤色に変更
//             // \e[0m でリセット（リセットしないと以降も赤くなる）
//             if(cell[y][x]){
// 	            fprintf(fp, "\e[31m#\e[0m");
//             }
//             else{
// 	            fprintf(fp, " ");
//             }
//         }
//         fprintf(fp,"|\n");
//     }

//     // 下の壁
//     fprintf(fp, "+");
//     for (int x = 0 ; x < width ; x++) {
//         fprintf(fp, "-");
//     }
//     fprintf(fp, "+\n");
    
//     fflush(fp); // バッファされている文字列を出力する
//     return; // stdlib.h で定義されている実行成功を表す整数マクロ: 実体は0
// }

void my_update_cells(const int height, const int width, int cell[height][width]) {
    int cellnext[height][width];
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            int cnt = 0;
            if (i-1>=0 && j-1>=0) {
                if (cell[i-1][j-1] == 1) {
                    ++cnt;
                }
            }
            if (i-1>=0) {
                if (cell[i-1][j] == 1) {
                    ++cnt;
                }
            }
            if (j-1>=0){
                if (cell[i][j-1] == 1) {
                    ++cnt;
                }
            }
            if (i-1>=0 && j+1<width) {
                if (cell[i-1][j+1] == 1) {
                    ++cnt;
                }  
            }
            if (i+1<height && j-1>=0) {
                if (cell[i+1][j-1] == 1) {
                    ++cnt;
                }  
            }
            if (i+1<height && j+1<width) {
                if (cell[i+1][j+1] == 1) {
                    ++cnt;
                }
            }
            if (j+1<width) {
                if (cell[i][j+1] == 1) {
                ++cnt;
                }
            }
            if(i+1<height){
                if (cell[i+1][j] == 1) {
                    ++cnt;
                }
            }
            if (cell[i][j] == 1) {
                if (cnt == 2 ||cnt == 3) {
                    cellnext[i][j] = 1;
                }
                else{
                    cellnext[i][j] = 0;
                }
            }
            if (cell[i][j] == 0) {
                if (cnt == 3) {
                    cellnext[i][j] = 1;
                }
                else{
                    cellnext[i][j] = 0;
                }
            }            
        }
    }
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            cell[i][j] = cellnext[i][j];
        }
    }    
}

void my_update_file(int gen, const int height, const int width, int cell[height][width]) {
    FILE *fp;
    char filename[100];
    sprintf(filename, "gen%04d.lif", gen);
    fp=fopen(filename, "w");
    fprintf(fp, "#Life 1.06\n");
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            if(cell[i][j]==1) {
                fprintf(fp, "%d %d\n", j, i);
            }
        }
    }
}