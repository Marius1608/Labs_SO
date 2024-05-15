#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "fifo_lab11_3"

int main() {

    char buffer[128],operation;
    int operand1, operand2;

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open FIFO");
        exit(1);
    }

    while (1) {
        printf("Enter a and b and an operation + or - , or 'w' to exit: ");
        scanf("%d %d %c", &operand1, &operand2, &operation);

        if (operation == 'w') {
            break;
        }

        sprintf(buffer, "%d %d %c", operand1, operand2, operation);
        if (write(fd, buffer, strlen(buffer) + 1) == -1) {
            perror("Failed to write to FIFO");
             exit(1);
        }

    }

    close(fd);
    return 0;
}
