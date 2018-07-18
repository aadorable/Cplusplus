#include <stdio.h>
#include <stdlib.h>

#define N 4
#define N 4

void rotate(int arr[N][N], int a, int b, int c, int d){
    for(int i = 0; i < d - b; ++i){
        int tmp = arr[a][b + i];
        arr[a][b + i] = arr[c - i][b];
        arr[c - i][b] = arr[c][d - i];
        arr[c][d - i] = arr[a + i][d];
        arr[a + i][d] = tmp;
    }
}

void rotateArr(int arr[N][N]){
    int a = 0;
    int b = 0;
    int c = N - 1;
    int d = N - 1;
    while(a < c){
        rotate(arr, a++, b++, c--, d--);
    }
}

void Show(int arr[N][N]){
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            printf("%3d", arr[i][j]);
        }
        printf("\n");
    }
}

int main(){
    int arr[N][N] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    Show(arr);
    rotateArr(arr);
    printf("\n");
    Show(arr);
    return 0; 
}


//int main(){
//    for(int i = 1; i <= 9; ++i){
//        printf("\033[3%dm", i - 1);
//        //printf("\033[4%dm", (i + 1) % 8);
//        for(int j = 1; j <= i; ++j){
//            printf("%d * %d = %-3d", j, i, j * i);
//        }
//        printf("\n");
//    }
//    printf("\033[0m");
//    return 0;
//}
