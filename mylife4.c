#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // sleep()関数を使う
#include <string.h>
#include <assert.h>
//#include "gol.h"

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp);
void my_print_cells(FILE* fp, int gen, const int height, const int width, int cell[height][width]);
void my_update_cells(const int height, const int width, int cell[height][width]);
void my_update_file(int gen, const int height, const int width, int cell[height][width]);
void my_init_RLE_cells(const int height, const int width,int cell[height][width],FILE* fp);
void my_merge_cells(const int height, const int width, int cell[height][width], int cell1[height][width], int cell2[height][width]);

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

  int cell1[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell1[y][x] = 0;
    }
  }

  int cell2[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell2[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 3 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 3) {
    FILE *lgfile1;
    FILE *lgfile2;
    if ( (lgfile1 = fopen(argv[1],"r")) != NULL ) {
        char buff[100];
        char buff2[100] = "#Life 1.06\n";
        fgets(buff, 100, lgfile1);
        if (strcmp(buff, buff2) == 0) {
            //fprintf(stderr,"OK!\n");
            my_init_cells(height,width,cell1,lgfile1);
        }
        else if(buff[0] == '#' || buff[0] == 'x'){
            while (buff[0] == '#') {
                fgets(buff, 100, lgfile1);
                // printf("sentou: %c\n", buff[0]);
            }
            my_init_RLE_cells(height,width,cell1,lgfile1);
        } 
        else {
            assert(0 == 1);  
        }
      //my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile1);

    if ( (lgfile2 = fopen(argv[2],"r")) != NULL ) {
        char buff[100];
        char buff2[100] = "#Life 1.06\n";
        fgets(buff, 100, lgfile2);
        if (strcmp(buff, buff2) == 0) {
            //fprintf(stderr,"OK!\n");
            my_init_cells(height,width,cell2,lgfile2);
        }
        else if(buff[0] == '#' || buff[0] == 'x'){
            while (buff[0] == '#') {
                fgets(buff, 100, lgfile2);
                // printf("sentou: %c\n", buff[0]);
            }
            my_init_RLE_cells(height,width,cell2,lgfile2);
        } 
        else {
            assert(0 == 1);  
        }
      //my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[2]);
      return EXIT_FAILURE;
    }
    fclose(lgfile2);
  }
  else if(argc == 2){
    FILE *lgfile1;
    if ( (lgfile1 = fopen(argv[1],"r")) != NULL ) {
        char buff[100];
        char buff2[100] = "#Life 1.06\n";
        fgets(buff, 100, lgfile1);
        if (strcmp(buff, buff2) == 0) {
            //fprintf(stderr,"OK!\n");
            my_init_cells(height,width,cell1,lgfile1);
        }
        else if(buff[0] == '#' || buff[0] == 'x'){
            while (buff[0] == '#') {
                fgets(buff, 100, lgfile1);
                // printf("sentou: %c\n", buff[0]);
            }
            my_init_RLE_cells(height,width,cell1,lgfile1);
        } 
        else {
            assert(0 == 1);  
        }
      //my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile1);

    my_init_cells(height, width, cell2, NULL); // デフォルトの初期値を使う
  }
  else {
     my_init_cells(height, width, cell1, NULL);
     my_init_cells(height, width, cell2, NULL); 
  }

    my_merge_cells(height, width, cell, cell1, cell2);
    my_print_cells(fp, 0, height, width, cell); // 表示する
    sleep(1); // 1秒休止
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)

  /* 世代を進める*/
  int cnt = 100;
  for (int gen = 1 ;gen<10000; gen++) {  
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    if(gen == cnt) {
        cnt += 100;
        my_update_file(gen, height, width, cell);
    }
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
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
        // char bff[100];
        // fgets(bff, 100, fp);
        while (fscanf(fp, "%d %d", &a, &b) != EOF) {
            cell[b][a] = 1;
        }
    }
}

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]) {
    int cnt = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (cell[y][x] == 1) {
                ++cnt;
            }
        }
    }
    double ratio = 100 * (double)cnt/(height*width);
    //上の壁
    fprintf(fp,"generation = %d ratio = %.2f%%\n", gen, ratio); // この場合 (fp = stdout), printfと同じ
    fprintf(fp,"+");
    for (int x = 0 ; x < width ; x++) {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
    /* 外壁と 内側のゲーム部分 */
    for (int y = 0; y < height; y++) {
        fprintf(fp,"|");
        for (int x = 0; x < width; x++) {
            // ANSIエスケープコードを用いて、赤い"#" を表示
            // \e[31m で 赤色に変更
            // \e[0m でリセット（リセットしないと以降も赤くなる）
            if(cell[y][x]==1){
	            fprintf(fp, "\e[31m#\e[0m");
            }
            else if(cell[y][x]==-1){
	            fprintf(fp, "\e[32m#\e[0m");
            }
            else{
	            fprintf(fp, " ");
            }
        }
        fprintf(fp,"|\n");
    }

    // 下の壁
    fprintf(fp, "+");
    for (int x = 0 ; x < width ; x++) {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
    
    fflush(fp); // バッファされている文字列を出力する
    return; // stdlib.h で定義されている実行成功を表す整数マクロ: 実体は0
}

void my_update_cells(const int height, const int width, int cell[height][width]) {
    int cellnext[height][width];
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            int cnt1 = 0;
            int cnt2 = 0;
            if (i-1>=0 && j-1>=0) {
                if (cell[i-1][j-1] == 1) {
                    ++cnt1;
                }
                else if (cell[i-1][j-1] == -1) {
                    ++cnt2;
                }
            }
            if (i-1>=0) {
                if (cell[i-1][j] == 1) {
                    ++cnt1;
                }
                else if (cell[i-1][j] == -1) {
                    ++cnt2;
                }
            }
            if (j-1>=0){
                if (cell[i][j-1] == 1) {
                    ++cnt1;
                }
                else if (cell[i][j-1] == -1) {
                    ++cnt2;
                }
            }
            if (i-1>=0 && j+1<width) {
                if (cell[i-1][j+1] == 1) {
                    ++cnt1;
                }
                else if (cell[i-1][j+1] == -1) {
                    ++cnt2;
                }  
            }
            if (i+1<height && j-1>=0) {
                if (cell[i+1][j-1] == 1) {
                    ++cnt1;
                } 
                else if (cell[i+1][j-1] == -1) {
                    ++cnt2;
                }  
            }
            if (i+1<height && j+1<width) {
                if (cell[i+1][j+1] == 1) {
                    ++cnt1;
                }
                else if (cell[i+1][j+1] == -1) {
                    ++cnt2;
                }
            }
            if (j+1<width) {
                if (cell[i][j+1] == 1) {
                ++cnt1;
                }
                else if (cell[i][j+1] == -1) {
                ++cnt2;
                }
            }
            if(i+1<height){
                if (cell[i+1][j] == 1) {
                    ++cnt1;
                }
                else if (cell[i+1][j] == -1) {
                    ++cnt2;
                }
            }
            if (cell[i][j] == 1) {
                if (cnt1 == 2 ||cnt1 == 3) {
                    cellnext[i][j] = 1;
                }
                else{
                    cellnext[i][j] = 0;
                }
            }
            if (cell[i][j] == -1) {
                if (cnt2 == 2 ||cnt2 == 3) {
                    cellnext[i][j] = -1;
                }
                else{
                    cellnext[i][j] = 0;
                }
            }
            if (cell[i][j] == 0) {
                if (cnt1 == 3 && cnt2 == 3) {
                    int c  = rand() % 2;
                    if(c == 0) {
                        cellnext[i][j] = 1;
                    }
                    else {
                        cellnext[i][j] = -1;
                    }
                }
                else if (cnt1 == 3){
                    cellnext[i][j] = 1;
                }
                else if (cnt2 == 3){
                    cellnext[i][j] = -1;
                }
                else {
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

void my_init_RLE_cells(const int height, const int width,int cell[height][width],FILE* fp){
    int a = 0; //cellのx座標
    int b = 0; //cellのy座標
    char c;
    const int run_cnt_default = -1;
    int run_cnt = run_cnt_default; //<run_count>
    while ((c = fgetc(fp)) != '!') {
        //fprintf(stderr, "%c\n", c);
        if (c == 'b') {
            for (int i = 0; i < abs(run_cnt); ++i) {
                cell[b][a+i] = 0;
            }
            //printf("b: run_cnt= %d\n", run_cnt);
            a += abs(run_cnt);
            run_cnt = run_cnt_default;
        } else if (c == 'o') {
            for (int i = 0; i < abs(run_cnt); ++i) {
                cell[b][a+i] = 1;
            }
            a += abs(run_cnt);
            run_cnt = run_cnt_default;
        } else if (c == '$') {
            for (int i = a; i < width; ++i) {
                cell[b][i] = 0;
            }
            //printf("%d, %d\n", a, b);
            a = 0;
            b++;
            run_cnt = run_cnt_default;
                // fprintf(stderr, "%d, %d\n", i, width);
            
        } else if(c != '\n'){
            if (run_cnt == run_cnt_default) {
                run_cnt = (int)(c - '0');
            }
            else {
                run_cnt *= 10;
                run_cnt += ((int)(c - '0'));
            }
        }
    }
}

void my_merge_cells(const int height, const int width, int cell[height][width], int cell1[height][width], int cell2[height][width]) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(cell1[i][j]==0 && cell2[i][j]==0) {
                cell[i][j] = 0;
            }
            else if(cell1[i][j]==1 && cell2[i][j]==0) {
                cell[i][j] = 1;
            }
            else if(cell1[i][j]==0 && cell2[i][j]==1) {
                cell[i][j] = -1;
            }
            else {
                int c  = rand() % 2;
                if(c == 0) {
                    cell[i][j] = 1;
                }
                else {
                    cell[i][j] = -1;
                }
            }
        }
    }
}