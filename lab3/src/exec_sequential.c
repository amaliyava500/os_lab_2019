#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    return 1;
  } else if (pid == 0) {
    char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};
    execv("./sequential_min_max", args);

    perror("execv failed");
    exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);
    printf("Child process finished with status: %d\n", status);
  }

  return 0;
}