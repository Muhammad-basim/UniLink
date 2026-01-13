#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/admin.h"
#include "../include/fileio.h"
#include "../include/utils.h"

int main(void) {
	system("mode con: cols=150 lines=40");
    char choice_str[8];
    
    while (1) {
    	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // This is the function that gets the colors from library window.h
	    SetConsoleTextAttribute(h, 14);   // Yellow color
	    main_banner();
	    SetConsoleTextAttribute(h, 10);
        printf("\n===== UniLink Login =====\n");
        printf("1. Admin Login\n");
        printf("2. Teacher Login\n");
        printf("3. Student Login\n");
        printf("0. Exit\n");
        read_line("Enter choice", choice_str, sizeof(choice_str));
	    if (!is_number(choice_str)) {
	    	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        system("cls");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice_str);
        if (c == 1) {
            if (admin_login()) {
            	system("cls");
                printf("\nLogin successful!\n");
                admin_menu();
            } else {
            	SetConsoleTextAttribute(h, 12);
            	system("cls");
                printf("Invalid admin login!\n");
                SetConsoleTextAttribute(h, 10);
            }
        }
        else if (c == 2){
        	char teacher_name[50];
        	if (teacher_login(teacher_name, sizeof(teacher_name))) {
        		system("cls");
    		    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			    SetConsoleTextAttribute(h, 14);
			    teacher_banner();
				SetConsoleTextAttribute(h, 10);
		        printf("\nWelcome, %s!\n", teacher_name);
		        teacher_menu();
		    } else {
		    	SetConsoleTextAttribute(h, 12);
		    	system("cls");
		        printf("Invalid teacher login!\n");
		        SetConsoleTextAttribute(h, 10);
		    }
		}
		else if (c == 3){
        	char student_name[50];
        	if (student_login(student_name, sizeof(student_name))) {
		        printf("\nWelcome, %s!\n", student_name);
		        student_menu();
		    } else {
		    	SetConsoleTextAttribute(h, 12);
		    	system("cls");
		        printf("Invalid Student login!\n");
		        SetConsoleTextAttribute(h, 10);
		    }
		}
        else if (c == 0){
        	SetConsoleTextAttribute(h, 14);
        	end_banner();
        	SetConsoleTextAttribute(h, 7);
        	break;	
		} 
        else{
        	SetConsoleTextAttribute(h, 12);
        	system("cls");
        	printf("Invalid choice.\n");
        	SetConsoleTextAttribute(h, 10);
		} 
    }
    return 0;
}

void main_banner(){
	printf("  _   _       _   _     _       _      \n");
	printf(" | | | |_ __ (_) | |   (_)_ __ | | __  \n");
	printf(" | | | | '_ \\| | | |   | | '_ \\| |/ /  \n");
	printf(" | |_| | | | | | | |___| | | | |   <   \n");
	printf("  \\___/|_| |_|_| |_____|_|_| |_|_|\\_\\  \n");                                     
}

void end_banner(){
	printf("  _____ _                 _                             __                         _                _   _       _ _     _       _     \n");
	printf(" |_   _| |__   __ _ _ __ | | __   _   _  ___  _   _    / _| ___  _ __    _   _ ___(_)_ __   __ _   | | | |_ __ (_) |   (_)_ __ | | __ \n");
	printf("   | | | '_ \\ / _` | '_ \\| |/ /  | | | |/ _ \\| | | |  | |_ / _ \\| '__|  | | | / __| | '_ \\ / _` |  | | | | '_ \\| | |   | | '_ \\| |/ / \n"); 
	printf("   | | | | | | (_| | | | |   <   | |_| | (_) | |_| |  |  _| (_) | |     | |_| \\__ \\ | | | | (_| |  | |_| | | | | | |___| | | | |   <  \n"); 
	printf("   |_| |_| |_|\\__,_|_| |_|_|\\_\\   \\__, |\\___/ \\__,_|  |_|  \\___/|_|      \\__,_|___/_|_| |_|\\__, |   \\___/|_| |_|_|_____|_|_| |_|_|\\_\ \n"); 
	printf("                                  |___/                                                    |___/                                     			 \n");
}

void teacher_banner(){
	printf("  _____               _                  __  __                  \n");
	printf(" |_   _|__  __ _  ___| |__   ___ _ __   |  \\/  | ___ _ __  _   _ \n");
	printf("   | |/ _ \\/ _` |/ __| '_ \\ / _ \\ '__|  | |\\/| |/ _ \\ '_ \\| | | |\n");
	printf("   | |  __/ (_| | (__| | | |  __/ |     | |  | |  __/ | | | |_| |\n");
	printf("   |_|\\___|\\__,_|\\___|_| |_|\\___|_|     |_|  |_|\\___|_| |_|\\__,_|\n");                                                 
}

