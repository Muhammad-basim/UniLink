#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/utils.h"
#include "../include/student.h"

#define STUDENTS_FILE "data/students.csv"
#define MAX_COURSES 20
// Global to store current student name (optional)
char CURRENT_STUDENT_NAME[50];
char CURRENT_STUDENT_ID[20];
// --- Student Login ---
int student_login(char *name_buffer, int size) {
    char id[50], pass[50];
    char line[256];
    printf("\n--- Student Login ---\n");
    read_line("Enter Student ID", id, sizeof(id));
    read_password(pass, sizeof(pass), "Enter Password");
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
        printf("Error: students.csv not found!\n");
        return 0;
    }
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *sid = strtok(copy, ",");
        char *sname = strtok(NULL, ",");
        strtok(NULL, ",");  // skip department
        char *spass = strtok(NULL, ",\n");
        if (!sid || !sname || !spass) continue;
        if (strcmp(id, sid) == 0 && strcmp(pass, spass) == 0) {
            strncpy(name_buffer, sname, size - 1);
            name_buffer[size - 1] = '\0';
            strcpy(CURRENT_STUDENT_ID, id);
            strncpy(CURRENT_STUDENT_NAME, sname, sizeof(CURRENT_STUDENT_NAME)-1);
            fclose(f);
            return 1; // login success
        }
    }
    fclose(f);
    return 0; // login failed
}
// --- Student Menu ---
void student_menu(const char *studentID) {
    char choice[10];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 14);
    student_banner();
	SetConsoleTextAttribute(h, 10);
    while (1) {
        printf("\n--- Student Menu ---\n");
        printf("1. View My Enrolled Courses\n");
        printf("2. View Marks\n");
        printf("3. Attempt Quiz\n");
        printf("4. View Quiz Result\n");
        printf("5. Change Password\n");
        printf("6. View My GPA\n");
        printf("0. Logout\n");
        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
            continue;
        }
        int c = atoi(choice);
        switch (c) {
            case 1:
                view_student_courses(CURRENT_STUDENT_ID);
                break;
            case 2:
                view_student_marks(CURRENT_STUDENT_ID);
                break;
            case 3:
                attempt_quiz(CURRENT_STUDENT_ID);
                break;
            case 4:
                view_quiz_result(CURRENT_STUDENT_ID);
                break;
            case 5:
                student_change_password(CURRENT_STUDENT_ID);
                break;
            case 6:
		        view_student_gpa(CURRENT_STUDENT_ID);
		        break;
            case 0:
            	printf("Logging out...\n");
				system("cls");
                return;
            default:
            	SetConsoleTextAttribute(h, 12);
	        	printf("Invalid choice. Please select a valid menu option.\n");
	        	SetConsoleTextAttribute(h, 10);
        }
    }
}



