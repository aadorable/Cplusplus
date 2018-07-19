#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int c;
    int i = 0;
    char buf[1024] = {};

    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    old.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    while((c = getchar()) != '\n'){
        buf[i++] = c;
        putchar('*');
    }
    printf("\n");
    old.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    printf("密码：%s\n", buf);
    return 0;
}
