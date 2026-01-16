#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "my_fifo"
#define MAX_LEN 100
#define TARGET_COUNT 5

int main() {
    int fd;
    char buffer[MAX_LEN + 1];
    int length_counts[MAX_LEN + 1] = {0};

    unlink(FIFO_NAME);
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    printf("Сервер запущен. Жду клиента...\n");

    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    while (1) {
        ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        int len = strlen(buffer);

        if (len >= 1 && len <= MAX_LEN) {
            length_counts[len]++;
            printf("Получена строка длиной %d. Всего таких: %d\n", len, length_counts[len]);

            if (length_counts[len] >= TARGET_COUNT) {
                printf("Цель достигнута! 5 строк длины %d получено. Завершаюсь...\n", len);
                break;
            }
        }
    }

    close(fd);
    unlink(FIFO_NAME);
    return EXIT_SUCCESS;
}