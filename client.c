#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define FIFO_NAME "my_fifo"

void send_random_string(int fd) {
    int len = (rand() % 100) + 1;
    char str[101];

    for (int i = 0; i < len; i++) {
        str[i] = 'a' + (rand() % 26);
    }
    str[len] = '\0';

    write(fd, str, len);
    printf("Отправлена строка длиной: %d\n", len);
}

int main() {
    srand(time(NULL));

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Не удалось открыть FIFO. Запустите сначала сервер");
        return EXIT_FAILURE;
    }

    while (1) {
        send_random_string(fd);
        usleep(100000);
    }

    close(fd);
    return EXIT_SUCCESS;
}