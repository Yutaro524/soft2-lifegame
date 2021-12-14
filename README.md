# ライフゲーム課題

mylife3.cに課題１〜３まで含まれています。

## 課題該当箇所のプログラムについての説明

### 課題１

my_init_cellsの前半部分が対応する。

```c
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

        else ...
```
- 乱数を読み込む際、現在時間を取り込むことで出力が毎回変わる設定になっている。
- aに0~1の一様乱数を1000個読み込み、1000で割ることで平均値、約0.5を作成し、0.2倍して約0.1の乱数を作った。
- 乱数なので同じcell[height][width]に偶然入ることもあるため、もしcell[height][width]がすでに1ならばもう一度ループするように設計した。


### 課題２
my_update_fileという関数を作成した。
```c
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
```
- char*バージョンでファイル名を作成し、ファイルの1行目に#Life 1.06\nを、二行目以降に生きているセルの行と列を書き込むようにした。

世代を進める箇所は以下の通り
```c
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
  ```
- ファイル出力のみ100世代ごとなので100ごとにカウントする変数cntを作り、gen==cntとなる時のみmy_update_fileを実行する仕様にした。

### 課題３
rleフォーマットを読み込めるようにmy_init_RLE_cellsを作成した。
```c
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
```
- run_cntは数字がない時は1となるようにしたかったが、1文字ずつ読むため、例えば12bとかの時に１と区別がつかなくて困る。そこでrun_cnt_default=-1として絶対値を用いた。
- 読み込んだ数字はchar型であり、キャストする際にASCIIコードを考える必要があったがなかなか気づかずにエラーを連発してしまった。

ファイルを読み込む際のlifファイルとrleファイルの区別はlifファイルの先頭行で判別した。
```c
FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
        char buff[100];
        char buff2[100] = "#Life 1.06\n";
        fgets(buff, 100, lgfile);
        if (strcmp(buff, buff2) == 0) {
            //fprintf(stderr,"OK!\n");
            my_init_cells(height,width,cell,lgfile);
        }
        else if(buff[0] == '#' || buff[0] == 'x'){
            while (buff[0] == '#') {
                fgets(buff, 100, lgfile);
                // printf("sentou: %c\n", buff[0]);
            }
            my_init_RLE_cells(height,width,cell,lgfile);
        } else {
            assert(0 == 1);  
        }
```
- 読み込む際にrleファイルの方は#の行を全て飛ばすようにした。
    