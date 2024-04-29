#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif

#ifndef TOKEN_BUF_SIZE
    #define TOKEN_BUF_SIZE 64
#endif

void check_buffer(char** buffer) {
    if(*(buffer) == NULL) {
        printf("Error trying to allocate memory.");
        exit(EXIT_FAILURE);
    }
}

char* read_line() {
    size_t buffer_size = BUFFER_SIZE;
    char* buffer = (char*) malloc(sizeof(char) * buffer_size);

    check_buffer(&buffer);

    for(int p = 0, c;;p++) {
        c = getchar();

        if(c == EOF || c == '\n') {
            buffer[p] = '\0';
            return buffer;
        }

        buffer[p] = c;

        if(p >= buffer_size) {
            buffer_size += BUFFER_SIZE;
            buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);

            check_buffer(&buffer);
        }
    }
}

char** split_lines(char* line) {
    size_t buffer_size = TOKEN_BUF_SIZE;
    char** buffer = (char**) malloc(sizeof(char*) * buffer_size);

    check_buffer(buffer);

    char* token = strtok(line, " \n\t\r\0");
    int i = 0;

    while(token != NULL) {
        buffer[i] = token;

        if(i >= buffer_size) {
            buffer_size += BUFFER_SIZE;
            buffer = (char**) realloc(buffer, sizeof(char*) * buffer_size);

            check_buffer(buffer);
        }

        token = strtok(NULL, " \n\t\r\0");        
        i++;
    }

    buffer[i] = NULL;
    return buffer;
}