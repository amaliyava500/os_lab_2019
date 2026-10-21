#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main() {
    pid_t child_pid = fork();
    if (child_pid > 0) {
        printf("Родитель (PID %d) засыпает на 15 секунд\n", getpid());
        printf("Дочерний процесс (PID %d) сейчас станет зомби\n", child_pid);
        printf("Введи в другом терминале: ps aux | grep 'Z'\n");
        sleep(15);
        printf("Родитель проснулся и завершается. Зомби исчезнет\n");
    } else {
        exit(0);
    }
    return 0;
}