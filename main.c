#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // sleep()関数を使う
#include <string.h>

void my_init_RLE_cells(const int height, const int width,int cell[height][width],FILE* fp){
    int a = 0; //cellのx座標
    int b = 0; //cellのy座標
    char c;
    int d = 1; //<run_count>
    while ((c = fgetc(fp)) != '!') {
        if (c == 'b') {
            for (int i = 0; i < d; ++i) {
                cell[b][a+i] = 0;
            }
            a += d;
            d = 1;
        }
        else if (c == 'o') {
            for (int i = 0; i < d; ++i) {
                cell[b][a+i] = 1;
            }
            a += d;
            d = 1;
        }
        else if (c == '$') {
            for (int i = a; i < width; ++i) {
                cell[b][i] = 0;
                a = 0;
                b++;
            }
        }
        else if(c != '\n'){
            if (d == 1) {
                d = (int)c;
            }
            else {
                d = 10 * d + (int)c;
            }
        }
    }
}

int main(int argc, char **argv) {
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
        char buff[100];
        fgets(buff, 100, lgfile);
    }
    my_init_RLE_cells(height,width,cell,lgfile);
  }
  printf("%d", cell[0][0]);
}