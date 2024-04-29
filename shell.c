#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "src/parser.h"
#include "src/colors.h"
#include "src/commands.h"

int main(int argc, char* argv[]) {
    #ifndef _WIN32
        printf("This shell can only be used on Windows for now.");
        exit(0);
    #endif

    system("cls");

    SetConsoleColor(MAGENTA_COLOR);
    printf("- ");

    SetConsoleColor(LIGHT_MAGENTA_COLOR);
    printf("s3s is an open source shell by s3lee for study purposes. (Use the 'help' command to see the list of commands)\n");

    char* path = (char*)malloc(sizeof(char) * MAX_PATH);
    char buffer[MAX_PATH];

    DWORD r = GetCurrentDirectory(MAX_PATH, buffer);

    if(r != 0) {
        strcpy(path, buffer);
    } else {
        SetConsoleColor(LIGHT_RED_COLOR);
        printf("!> Error trying to find current directory.\n");
        SetConsoleColor(RESET_COLOR);

        exit(EXIT_FAILURE);
    }

    for(;;) {
        SetConsoleColor(CYAN_COLOR);
        printf("\n~# ");

        SetConsoleColor(LIGHT_CYAN_COLOR);

        char* cmd = read_line();
        char** lines = split_lines(cmd);

        int arg_c = 0;
        while(lines[arg_c] != NULL) arg_c++;

        SetConsoleColor(RESET_COLOR);

        int result = execute(arg_c, lines, &path);

        if(result == 1) {
            break;
        }

        free(cmd);
        free(lines);
    }

    free(path);

    return EXIT_SUCCESS;
}