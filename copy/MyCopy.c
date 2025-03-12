#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BAFFER_DEFAULT_SIZE 64

void copy(FILE *src, FILE *dest, const size_t buffer_size) {
    // printf("current buffer size: %d\n", buffer_size);
    char *inputBuffer = (char *)malloc(buffer_size*sizeof(char));
    if (inputBuffer == NULL) {
        perror("Error allocating memory for input buffer");
        exit(EXIT_FAILURE);
    }
    
    size_t bytesRead;

    // Read data in chunks and write to the destination file.
    while ((bytesRead = fread(inputBuffer, 1, sizeof(inputBuffer), src)) > 0) {
        if (fwrite(inputBuffer, 1, bytesRead, dest) != bytesRead) {
            perror("Error writing to destination file");
            exit(EXIT_FAILURE);
        }
    }

    free(inputBuffer);
    // Check for read error.
    if (ferror(src)) {
        perror("Error reading source file--------------****");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 4) {
        fprintf(stderr, "argument error, with usage: %s <source_file> <destination_file> <buffer size>\n", argv[0]);
        exit(1);
    } else if (argc < 3) {
        fprintf(stderr, "argument error, with usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(1);
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        perror("Error opening destination file");
        fclose(src);
        exit(EXIT_FAILURE);
    }
    size_t buffer_size;
    if (argc == 4) {
        buffer_size = atoi(argv[3]);
    } else {
        buffer_size = BAFFER_DEFAULT_SIZE;
    }

    copy(src, dest, buffer_size);

    fclose(src);
    fclose(dest);
    return 0;
}
