#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int copy_file(const char *source_file, const char *destination_file) {
    int fd_source = -1, fd_destination = -1;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // Deschide fisierul sursa pentru citire
    fd_source = open(source_file, O_RDONLY);
    if (fd_source == -1) {
        perror("Eroare la deschiderea fisierului sursa");
        return EXIT_FAILURE;
    }

    // Creeaza sau deschide fisierul destinatie pentru scriere
    fd_destination = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_destination == -1) {
        perror("Eroare la deschiderea fisierului destinatie");
        close(fd_source);  // Inchide fisierul sursa
        return EXIT_FAILURE;
    }

    // Citeste din fisierul sursa si scrie in fisierul destinatie
    while ((bytes_read = read(fd_source, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(fd_destination, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Eroare la scrierea in fisierul destinatie");
            close(fd_source);  // Inchide ambele fisiere
            close(fd_destination);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read == -1) {
        perror("Eroare la citirea din fisierul sursa");
        close(fd_source);  // Inchide ambele fisiere
        close(fd_destination);
        return EXIT_FAILURE;
    }

    // Inchide fisierele
    close(fd_source);
    close(fd_destination);

    printf("Copierea fisierului s-a realizat cu succes.\n");

    return EXIT_SUCCESS;
}

int main() {
    const char *source_file = "/usr/include/stdlib.h";
    const char *destination_file = "copy_stdlib.h";

    if (copy_file(source_file, destination_file) != EXIT_SUCCESS) {
        printf("Copierea fisierului a esuat.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
