#include <stdio.h>
#include <stdlib.h>
#include "../include/admin.h"
#include "../include/fileio.h"
#include "../include/utils.h"

int main(void) {
    char choice_str[8];
    while (1) {
        printf("\n===== UniLink Login =====\n");
        printf("1. Admin Login\n");
        printf("2. Teacher Login\n");
        printf("3. Student Login\n");
        printf("0. Exit\n");
        read_line("Enter choice", choice_str, sizeof(choice_str));
	    if (!is_number(choice_str)) {
	        printf("Invalid choice (numbers only).\n");
	        continue;
	    }
        int c = atoi(choice_str);
        if (c == 1) {
            if (admin_login()) {
                printf("\nLogin successful!\n");
                admin_menu();
            } else {
                printf("Invalid admin login!\n");
            }
        }
        else if (c == 2){
        	char teacher_name[50];
        	if (teacher_login(teacher_name, sizeof(teacher_name))) {
		        printf("\nWelcome, %s!\n", teacher_name);
		        teacher_menu();
		    } else {
		        printf("Invalid teacher login!\n");
		    }
		}
		else if (c == 3){
        	char student_name[50];
        	if (student_login(student_name, sizeof(student_name))) {
		        printf("\nWelcome, %s!\n", student_name);
		        student_menu();
		    } else {
		        printf("Invalid Student login!\n");
		    }
		}
        else if (c == 0) break;
        else printf("Invalid choice.\n");
    }
    return 0;
}

