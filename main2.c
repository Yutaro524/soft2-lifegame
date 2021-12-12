#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    // int cell[40][70];
    // char filename[] = "gosperglidergun.lif";

    // // ここからがファイル読み込みの定型文
    // FILE *fp;
    // fp = fopen(filename, "r");  // "read"
    // if (fp == NULL) {
    //     printf("Cannot open %s\n", filename);
    //     return -1;

    // }

    // // ここでfpを経由してファイルからデータを読み込む
    // int a, b;
    // char bff[100];
    // fgets(bff, 100, fp);
    // while (fscanf(fp, "%d %d", &a, &b) != EOF) {
    //     cell[a][b] = 1;
    // }

    // for (int i=0; i<70; ++i) {
    //     for (int j=0; j<40; ++j) {
    //         printf("%d ", cell[j][i]);
    //     }
    //     printf("\n");

    // }

    // // ここでfpの後始末
    // fclose(fp);
    srand((unsigned)time(NULL));
    double a = 0;
    for (int i=0; i<1000; ++i) {
        a += (double)rand()/RAND_MAX;
    }
    double b = (a * 0.001 * 0.2);
    int c = b * 40*70;
    b = (double)c/2800;
    printf("%f", b);

}