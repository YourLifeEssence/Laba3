#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    const char *message = "Привет из родительского процесса!";
    char buffer[1024];

    if (pipe(fd) == -1) {
        perror("Ошибка при вызове pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Ошибка при вызове fork");
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        close(fd[0]);

        printf("Родитель: Отправляю строку в канал...\n");
        if (write(fd[1], message, strlen(message) + 1) == -1) {
            perror("Родитель: ошибка записи");
        }
        close(fd[1]);

        wait(NULL);
    }
    else {
        close(fd[1]);

        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer));
        if (bytesRead > 0) {
            printf("Дочерний процесс получил: %s\n", buffer);
        } else if (bytesRead == -1) {
            perror("Дочерний процесс: ошибка чтения");
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}