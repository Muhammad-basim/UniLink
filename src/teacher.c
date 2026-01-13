#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/utils.h"
#include "../include/teacher.h"
//#include "teacher_fileio.c"

extern char CURRENT_TEACHER_ID[20];

int teacher_login(char *name_buffer, int size) {
    char id[50], pass[50];
    char line[256];

    printf("\n--- Teacher Login ---\n");
    read_line("Enter Teacher ID", id, sizeof(id));
    //read_line("Enter Password", pass, sizeof(pass));
    read_password(pass, sizeof(pass), "Enter Password");

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

		    /* ?? STORE LOGGED-IN TEACHER ID */
		    strcpy(CURRENT_TEACHER_ID, tid);
		
		    strncpy(name_buffer, tname, size - 1);
		    name_buffer[size - 1] = '\0';
		
		    fclose(f);
		    return 1;
		}
    }

    fclose(f);
    return 0;  // login failed
}

void teacher_menu(void) {
    char choice[10];
    while (1) {
        printf("\n--- Teacher Menu ---\n");
        printf("1. View My Assigned Courses\n");
        printf("2. View Students in a Course\n");
        printf("3. Upload Marks (Course-wise)\n");
        printf("4. View Course-wise Student Marks\n");
        printf("5. Change Password\n");
        printf("6. Manage Quiz\n");
        printf("0. Logout\n");

        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	system("cls");
	        printf("Invalid choice (numbers only).\n");
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            view_my_assigned_courses(CURRENT_TEACHER_ID);
        }
		else if (c == 2) {
		    view_students_in_course(CURRENT_TEACHER_ID);
		}
		else if (c == 3) {
		    upload_marks(CURRENT_TEACHER_ID);
		}
		else if (c == 4) {
		    view_course_student_marks(CURRENT_TEACHER_ID);
		}
		else if (c == 5) {
		    change_teacher_password(CURRENT_TEACHER_ID);
		}
		else if (c == 6) {
			manage_quiz_menu(CURRENT_TEACHER_ID);
		    //create_quiz(CURRENT_TEACHER_ID);
		}
        else if (c == 0) {
        	system("cls");
            break;
        }
        else{
        	system("cls");	
        	printf("Invalid choice.\n");
		} 
    }
}


void manage_quiz_menu(const char *teacherID) {
    char choice[10];

    while (1) {
        printf("\n--- Manage Quiz ---\n");
        printf("1. Create Quiz\n");
        printf("2. View My Quizzes\n");
        printf("3. Delete Quiz\n");
        printf("4. Update Quiz\n");
        printf("0. Back\n");

        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
            printf("Invalid input.\n");
            continue;
        }

        int c = atoi(choice);

        if (c == 1) create_quiz(teacherID);
        else if (c == 2) view_my_quizzes(teacherID);
        else if (c == 3) delete_quiz(teacherID);
        else if (c == 4) update_quiz(teacherID);
        else if (c == 0) break;
        else printf("Invalid choice.\n");
    }
}






