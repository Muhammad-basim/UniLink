#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../include/utils.h"


char CURRENT_TEACHER_ID[20] = "";

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

void read_password(char *buffer, size_t size, const char *prompt) {
    printf("%s: ", prompt);
    size_t i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < size - 1) { // Enter = \r
        if (ch == '\b') { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            buffer[i++] = ch;
            printf("*"); // Mask input
        }
    }
    buffer[i] = '\0';
    printf("\n");
}

//float marks_to_gp(int marks) {
//    if (marks >= 85) return 4.00;
//    if (marks >= 80) return 3.70;
//    if (marks >= 75) return 3.30;
//    if (marks >= 70) return 3.00;
//    if (marks >= 65) return 2.70;
//    if (marks >= 60) return 2.30;
//    if (marks >= 50) return 2.00;
//    return 0.00;
//}


