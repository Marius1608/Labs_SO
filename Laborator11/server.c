#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "fifo_lab11_3"

int main() {

    char buffer[128],operation;
    int operand1, operand2;
    
    if (mkfifo(FIFO_NAME, 0600) == -1) {
        perror("Failed to create FIFO");
        exit(EXIT_FAILURE);
    }

    
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (read(fd, buffer, 128) == -1) {
            perror("Failed to read from FIFO");
            exit(EXIT_FAILURE);
        }
        sscanf(buffer, "%d %d %c", &operand1, &operand2, &operation);

        if (operation == 'w') {
            break;
        }

       
        int result;
        if (operation == '+') {
            result = operand1 + operand2;
        } else if (operation == '-') {
            result = operand1 - operand2;
        } else {
            strcpy(buffer, "Invalid operation");
        }

        sprintf(buffer, "%d", result);
        printf("Result: %s\n", buffer); 
    }

    
    close(fd);
    unlink(FIFO_NAME);

    return 0;
}
