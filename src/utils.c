#include <stdio.h>
#include <string.h>
#include "../include/utils.h"


void read_line(const char *prompt, char *buffer, int size) {
    if (prompt && prompt[0] != '\0')
        printf("%s: ", prompt);

    if (!fgets(buffer, size, stdin)) {
        buffer[0] = '\0';
        return;
    }

    char *p = strchr(buffer, '\n');
    if (p) *p = '\0';
}

int is_number(const char *s) {
    if (*s == '\0') return 0;       // empty string
    int i;
    for (i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0; 
    }
    return 1;
}

