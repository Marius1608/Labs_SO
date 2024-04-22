#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BUFFER_SIZE 1024

void listRec(char *path, char *name, char *string, int *nrOfExtension) {

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[1024];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Error");
        return ;
    }

    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath, name, string, nrOfExtension);

                } else if (strcmp(entry->d_name, name) == 0) {
                    
                    int fd = open(fullPath, O_RDONLY);
                    if (fd != -1) {
                        char buffer[BUFFER_SIZE];
                        ssize_t sizefile;
                        int ok = 0;
                        while ((sizefile = read(fd, buffer, BUFFER_SIZE)) > 0) {
                            if (strstr(buffer, string) != NULL) {
                                ok = 1;
                                break;
                            }
                        }
                        close(fd);
                        if (ok) {
                            char linkPath[1024];
                            snprintf(linkPath, sizeof(linkPath), "%s_link.%d",name, (*nrOfExtension)++);
                            symlink(fullPath, linkPath);
                            printf("Created symbolic link: %s -> %s\n", linkPath, fullPath);
                        }
                    }
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {

    if(argc != 4) {
        printf("Usage: %s <path> <name> <string>\n", argv[0]);
        return 1;
    }
    char *path = argv[1];
    char *name = argv[2];
    char *string = argv[3];
    int nrOfExtension = 1;
    listRec(path,name,string, &nrOfExtension);

    return 0;
}
