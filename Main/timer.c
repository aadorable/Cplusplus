#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <fcntl.h>

int main(){
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(tfd == -1){
        perror("fd_create"), exit(1);
    }
    struct itimerspec is;
    memset(&is, 0x00, sizeof is);
    is.it_interval.tv_sec = 2;     //每隔1秒执行一次
    is.it_value.tv_sec = 1;         //1秒后运行
    timerfd_settime(tfd, 0, &is, NULL);

    fd_set rset;

    int kfd = fileno(stdin);
    int maxfd = (kfd > tfd ? kfd : tfd) + 1;
    while(1){
        FD_ZERO(&rset);
        FD_SET(kfd, &rset);
        FD_SET(tfd, &rset);
        int nready = select(maxfd, &rset, NULL, NULL, NULL);
        if(nready == -1){
            continue;
        }
        if(FD_ISSET(tfd, &rset)){
            uint64_t a;
            read(tfd, &a, sizeof a);
            printf("定时器到\n");
        }
        if(FD_ISSET(kfd, &rset)){
            char buf[1024] = {};
            int n = read(kfd, buf, sizeof buf);
            if(n <= 0){
                break;
            }
            printf("buf = [%s]\n", buf);
        }
    }
    return 0;
}
