#include <stdio.h>

#define N 5

void infect(int arr[N][N], int i, int j){
    if(i < 0 || i >= N || j < 0 || j >= N || arr[i][j] != 1){
        return;
    }
    arr[i][j] = 2;
    infect(arr, i - 1, j);
    infect(arr, i, j + 1);
    infect(arr, i + 1, j);
    infect(arr, i, j - 1);
}

int land(int arr[N][N]){
    int island = 0;
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            if(arr[i][j] == 1){
                island++;
                infect(arr, i, j);
            }
        }
    }
    return island;
}

int main(){
    int arr[N][N] = {
        {1, 1, 1, 0 ,0},
        {1, 0, 0, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 1}
    };
    printf("%d\n", land(arr));
    return 0;
}
