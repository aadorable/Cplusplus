#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

void handler(int s){
    printf("recv %d\n", s);
}

int main(){
    signal(SIGALRM, handler);

    struct itimerval iv;
    memset(&iv, 0x00, sizeof iv);
    iv.it_interval.tv_sec = 1;     //每隔1秒执行一次
    iv.it_value.tv_sec = 3;         //3秒后运行
    setitimer(ITIMER_REAL, &iv, NULL);
    for(;;)
        pause();
}
