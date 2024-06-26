#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int main(int argc, char **argv)
{
    int fd;
    off_t size, i, write_pos = 0;
    char *data = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Could not open input file");
        return 1;
    }
    
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    data = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == (void*)-1) {
        perror("Could not map file");
        close(fd);
        return 1;
    }

   
    for (i = 0; i < size; i++) {
        if (!is_vowel(data[i])) {
            data[write_pos++] = data[i];
        }
    }


    memset(data + write_pos, ' ', size - write_pos);

    munmap(data, size);
    close(fd);

    return 0;
}
