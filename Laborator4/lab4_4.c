#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd;
    off_t size, crtPos, i;
    char c = 0;
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if(fd == -1) {
        perror("Could not open input file");
        return 1;
    }

	sscanf(argv[2], "%d" ,%crtPos);
	
	slen=strlen(arg[3]);
		

    size = lseek(fd, 0, SEEK_END);
    
    
    
    
    lseek(fd, 0, SEEK_SET);
    
    for(i= size ; i>=crtPos; i--) {
    	lseek(fd,0,SEEK_SET)
    	
    	
        if(read(fd, &c, 1) != 1) {
            perror("Reading error");
            break;
        }
        
        
            lseek(fd, i+slen, SEEK_SET);
            if(write(fd, &c, 1) != 1) {
                perror("Writing error");
                break;
            }
        }
    }

	if(write(fd,arg[3],slen)!=1){
		perror("Writing error");
	}
    close(fd);

    return 0;
}
