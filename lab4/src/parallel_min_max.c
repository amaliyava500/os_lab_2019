#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <signal.h>
#include "find_min_max.h"
#include "utils.h"

pid_t *child_pids = NULL;
int pnum_global = 0;

void kill_children(int sig) {
    printf("\n[ТАЙМАУТ] Время вышло Убиваем процессы\n");
    for (int i = 0; i < pnum_global; i++) {
        if (child_pids[i] > 0) {
            kill(child_pids[i], SIGKILL);
            printf("Убит процесс PID: %d\n", child_pids[i]);
        }
    }
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = false;
    int timeout = -1;

    while (true) {
        int current_optind = optind ? optind : 1;
        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {"timeout", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);
        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0: seed = atoi(optarg); break;
                    case 1: array_size = atoi(optarg); break;
                    case 2: pnum = atoi(optarg); break;
                    case 4: timeout = atoi(optarg); break;
                    default: printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f': with_files = true; break;
            case '?': break;
            default: printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) { printf("Has at least one no option argument\n"); return 1; }
    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    child_pids = malloc(sizeof(pid_t) * pnum);
    pnum_global = pnum;

    if (timeout > 0) {
        signal(SIGALRM, kill_children);
        alarm(timeout);
    }

    int active_child_processes = 0;
    int pipefd[2];
    if (!with_files) { pipe(pipefd); }

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                int begin = (array_size / pnum) * i;
                int end = (i == pnum - 1) ? array_size : (array_size / pnum) * (i + 1);
                struct MinMax min_max = GetMinMax(array, begin, end);

                if (with_files) {
                    char filename[256];
                    sprintf(filename, "data_%d.txt", i);
                    FILE *fp = fopen(filename, "w");
                    fprintf(fp, "%d %d", min_max.min, min_max.max);
                    fclose(fp);
                } else {
                    write(pipefd[1], &min_max, sizeof(struct MinMax));
                }
                return 0;
            } else {
                child_pids[i] = child_pid;
            }
        } else {
            printf("Fork failed\n");
            return 1;
        }
    }

    int status;
    while (active_child_processes > 0) {
        pid_t done = waitpid(-1, &status, WNOHANG);
        if (done > 0) {
            active_child_processes--;
            for (int i = 0; i < pnum; i++) {
                if (child_pids[i] == done) child_pids[i] = 0;
            }
        }
    }

    alarm(0);
    free(child_pids);
    free(array);
    printf("Родительский процесс завершен\n");
    return 0;
}