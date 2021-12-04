#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include "gol.h"

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp);
//void my_print_cells(FILE* fp, int gen, const int height, const int width, int cell[height][width]);
//void my_update_cells(const int height, const int width, int cell[height][width]);

int main(int argc, char **argv)
{
  FILE *fp = stdout;
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

  print_cells(fp, 0, height, width, cell); // 表示する
  sleep(1); // 1秒休止
  fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    update_cells(height, width, cell); // セルを更新
    print_cells(fp, gen, height, width, cell);  // 表示する
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}


void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp) {
    if (fp == NULL) {
        cell[30][20] = 1;
        cell[30][22] = 1;
        cell[31][22] = 1;
        cell[31][23] = 1;
        cell[32][20] = 1;
    }
    else {
        int a, b;
        char bff[100];
        fgets(bff, 100, fp);
        while (fscanf(fp, "%d %d", &a, &b) != EOF) {
            cell[a][b] = 1;
        }
    }


}

void print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]);