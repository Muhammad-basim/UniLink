#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/utils.h"

#define TEACHERS_FILE "data/teachers.csv"

int teacher_login(char *name_buffer, int size) {
    char id[50], pass[50];
    char line[256];

    printf("\n--- Teacher Login ---\n");
    read_line("Enter Teacher ID", id, sizeof(id));
    read_line("Enter Password", pass, sizeof(pass));

    FILE *f = fopen("data/teachers.csv", "r");
    if (!f) {
        printf("Error: teachers.csv not found!\n");
        return 0;
    }

    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);

        char *tid = strtok(copy, ",");
        char *tname = strtok(NULL, ",");
        strtok(NULL, ",");  // skip dept
        char *tpass = strtok(NULL, ",");

        if (!tid || !tname || !tpass) continue;

        char *newline = strchr(tpass, '\n');
        if (newline) *newline = '\0';

        if (strcmp(id, tid) == 0 && strcmp(pass, tpass) == 0) {
            // Copy name to buffer
            strncpy(name_buffer, tname, size - 1);
            name_buffer[size - 1] = '\0';
            fclose(f);
            return 1;  // login successful
        }
    }

    fclose(f);
    return 0;  // login failed
}


void teacher_menu(void) {
    char choice[10];
    while (1) {
        printf("\n--- Teacher Menu ---\n");
        printf("1. Create Quiz (coming soon)\n");
        printf("2. View Student Marks (coming soon)\n");
        printf("0. Logout\n");

        read_line("Enter choice", choice, sizeof(choice));
        int c = atoi(choice);

        if (c == 0) break;
        else printf("Feature not implemented yet.\n");
    }
}


