#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

off_t dirSize(const char *path) {


    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[1024];
    struct stat statbuf;
    off_t size = 0;

    
    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open");
        return -1;
    }

    while((entry = readdir(dir)) != NULL) {
        
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            
            snprintf(fullPath, 1024, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                size += statbuf.st_size;
                if(S_ISDIR(statbuf.st_mode)) {
                    dirSize(fullPath);
                }
            } else {
                perror("Error");
            }
        }
    }

    closedir(dir);
    return size;
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    off_t size = dirSize(argv[1]);
    if (size == -1) {
        perror("Error");
        return -1;
    }
    printf("Directory size %s: %lld bytes\n", argv[1], (long long)size);
    return 0;
}
