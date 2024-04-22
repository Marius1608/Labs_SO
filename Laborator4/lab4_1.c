#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int get_line(int fd, int lineNr, char *line, int maxLength) {
    int size, line_count, index_last_nl = -1, index_curr_nl = -1, bytes_readed = 0;
    char buf[BUF_SIZE];

    while (index_last_nl == -1 && index_curr_nl == -1) {
        size = read(fd, buf, BUF_SIZE);
        for (int i = 0; i < size; i++) {
            if (buf[i] == '\n') {
                line_count ++;
                if (line_count == lineNr) {
                    index_curr_nl = i + bytes_readed;
                    break;
                }
                else
                    index_last_nl = i + bytes_readed;
            }
        }
        bytes_readed += size;
    }
    lseek(fd, index_last_nl + 1, SEEK_SET);
    int len_line = index_curr_nl - index_last_nl;
    if (maxLength < len_line) {
        perror("Length too big");
        return -1;
    }
    size = read(fd, line, len_line);
    if (len_line != size) {
        perror("Read error");
        return -1;
    }
    line[size]='\0';
    return 0;
}

int main(int argc, char **argv) {
    int fd;
    char buf[1000];
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
    }

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("Could not open the input file");
        return -1;
    }
    if (get_line(fd, 2, buf, 1000) == 0)
        puts(buf);

    close(fd);
    return 0;
}
