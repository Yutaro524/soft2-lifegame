#include<stdio.h>
#include<string.h>
int convert_digit_to_int(const char c){
    return (int)(c - '0');
}

int main(){
    for(int i = 0; i <  100; i++){
        printf("ASCII: %d, character: %c\n", i,  (char)i);
    }
    char a[10] = "hoge";
    char b[20] = "hoge";
    printf("%d", strcmp(a, b));
    // int v = (int)'0';
    // printf("%d\n", v);
    // char c = (char)0;
    // printf("%c hogehoge\n", c);
    // printf("%d\n", convert_digit_to_int('0'));
    // printf("%d\n", convert_digit_to_int('2'));
    // printf("%d\n", convert_digit_to_int('4'));
    // printf("%d\n", convert_digit_to_int('7'));
    // printf("%d\n", convert_digit_to_int('9'));
}
