#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <direct.h>

#include "colors.h"

int execute(int argc, char** argv, char** path) {
    if(!strcmp(argv[0], "echo")) {
        int total_len = 0;
        for(int i = 1; i < argc; i++) {
            total_len += strlen(argv[i]);
        }

        char* buffer = (char*)malloc((total_len + argc) * sizeof(char));
        strcpy(buffer, argv[1]);

        for(int i = 2; i < argc; i++) {
            strcat(buffer, " ");
            strcat(buffer, argv[i]);
        }

        printf("%s\n", buffer);
        free(buffer);

    } else if(!strcmp(argv[0], "cd")) {
        DWORD atts = GetFileAttributes(argv[1]);

        if(atts != INVALID_FILE_ATTRIBUTES && atts & FILE_ATTRIBUTE_DIRECTORY) {
            strcpy(*path, argv[1]);
        } else {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Invalid directory.\n");
        }

    } else if(!strcmp(argv[0], "mkdir")) {
        char* directory_path = (char*) malloc(sizeof(char) * MAX_PATH);
        strcpy(directory_path, *path);
        strcat(directory_path, "\\");
        strcat(directory_path, argv[1]);

        if(_mkdir(directory_path) == -1) {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Error trying to create directory.\n");
        } else {
            SetConsoleColor(GREEN_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_GREEN_COLOR);
            printf("Directory created on %s.\n", directory_path);
        }

        free(directory_path);

    } else if(!strcmp(argv[0], "mkfile")) {
        FILE* file = fopen(argv[1], "w");

        if(file == NULL) {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Error trying to open file.\n");
        } else {
            if(argv[2] != NULL) {
                int total_len = 0;
                for(int i = 2; argv[i] != NULL; i++) {
                    total_len += strlen(argv[i]);
                }

                char* buffer = (char*) malloc((total_len + argc) * sizeof(char));
                strcpy(buffer, argv[2]);

                for(int i = 3; argv[i] != NULL; i++) {
                    strcat(buffer, " ");
                    strcat(buffer, argv[i]);
                }

                fprintf(file, buffer);

                free(buffer);
            }

            SetConsoleColor(GREEN_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_GREEN_COLOR);
            printf("The file has been created successfully.\n");

            SetConsoleColor(RESET_COLOR);

            fclose(file);
        }

    } else if(!strcmp(argv[0], "pwd")) {
        SetConsoleColor(GREEN_COLOR);
        printf("- ");

        SetConsoleColor(LIGHT_GREEN_COLOR);
        printf("%s\n", *path);

    } else if(!strcmp(argv[0], "ls")) {
        WIN32_FIND_DATA findData;
        HANDLE hFind;

        hFind = FindFirstFile("*", &findData);

        if(hFind == INVALID_HANDLE_VALUE) {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Error searching for files.\n");
        } else {
            SetConsoleColor(LIGHT_CYAN_COLOR);

            do {
                printf("%s\n", findData.cFileName);
            } while(FindNextFile(hFind, &findData) != 0);

            SetConsoleColor(RESET_COLOR);
        }

        FindClose(hFind);

    } else if(!strcmp(argv[0], "rmdir")) {
        if(RemoveDirectory(argv[1])) {
            SetConsoleColor(GREEN_COLOR);
            printf("-");

            SetConsoleColor(LIGHT_GREEN_COLOR);
            printf("The directory has been deleted successfully.\n");
        } else {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Error trying to delete directory.\n");
        }

    } else if(!strcmp(argv[0], "rm")) {
        if(remove(argv[1]) == 0) {
            SetConsoleColor(GREEN_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_GREEN_COLOR);
            printf("The file has been deleted successfully.\n");
        } else {
            SetConsoleColor(RED_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_RED_COLOR);
            printf("Error trying to delete file.\n");
        }

    } else if(!strcmp(argv[0], "call")) {
        FILE* file = fopen(argv[1], "r");

        if(file == NULL) {
            SetConsoleColor(GREEN_COLOR);
            printf("- ");

            SetConsoleColor(LIGHT_GREEN_COLOR);
            printf("Error trying to read file.\n");
        } else {
            char line[1024];

            while(fgets(line, 1024, file) != NULL) {
                char** lines = split_lines(line);
                int argc = 0;

                for(int i = 0; lines[i] != NULL; i++) {
                    argc++;
                }

                execute(argc, lines, path);
                free(lines);
            }
        }

    } else if(!strcmp(argv[0], "help")) {
        SetConsoleColor(LIGHT_MAGENTA_COLOR);

        printf("// cls -> Clears the terminal screen.\n"
            "// exit -> Exits the current command prompt.\n"
            "// call -> Executes an external file.\n"
            "// rm -> Removes (deletes) files.\n"
            "// rmdir -> Removes (deletes) directories.\n"
            "// ls -> Lists the files and directories in the current directory.\n"
            "// pwd -> Prints the current working directory.\n"
            "// mkfile -> Creates a new file.\n"
            "// mkdir -> Creates a new directory.\n"
            "// cd -> Changes the current directory.\n"
            "// echo -> Displays a message.\n");

        SetConsoleColor(RESET_COLOR);

    } else if(!strcmp(argv[0], "cls")) {
        system("cls");

    } else if(!strcmp(argv[0], "exit")) {
        return 1;
    }

    return 0;
}