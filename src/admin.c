#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/admin.h"
#include "../include/fileio.h"
#include "../include/utils.h"
int admin_login(void) {
    char user[50], pass[50];
    char line[100];
    printf("\n--- Admin Login ---\n");
    read_line("Username", user, sizeof(user));
    //read_line("Password", pass, sizeof(pass));
    read_password(pass, sizeof(pass), "Password");
    FILE *f = fopen("data/admin.txt", "r");
    if (!f) {
        printf("Error: admin.txt not found!\n");
        return 0;
    }
    while (fgets(line, sizeof(line), f)) {
        char *u = strtok(line, ",");
        char *p = strtok(NULL, ",");
        if (!u || !p) continue;
        char *newline = strchr(p, '\n');
        if (newline) *newline = '\0';
        if (strcmp(user, u) == 0 && strcmp(pass, p) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void admin_menu(void) {
    char choice[10];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    while (1) {
    	SetConsoleTextAttribute(h, 14);
		admin_banner();
		SetConsoleTextAttribute(h, 10);
        printf("\n=== Admin Menu ===\n");
        printf("1. Student Management\n");
        printf("2. Teacher Management\n");
        printf("3. Course Management\n");
        printf("0. Logout\n");

        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        system("cls");
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            manage_students();
        }
		else if (c == 2) {
		    manage_teachers();
		}
		else if (c == 3) {
		    manage_courses();
		}
        else if (c == 0) {
        	system("cls");
            break;
        }
        else{
        	SetConsoleTextAttribute(h, 12);
	        system("cls");
        	printf("Invalid choice.\n");
        	SetConsoleTextAttribute(h, 10);
		}
    }
}

void manage_students(void) {
	char choice[10];
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	while (1) {
        printf("\n--- Student Management ---\n");
        printf("1. Add Student Record\n");
        printf("2. List Students Record\n");
        printf("3. Update Student Record\n");
        printf("4. Delete Student Record\n");
        printf("5. Search Student Record\n");
        printf("6. GPA Calculation\n");
        printf("0. Back to Admin Menu\n");
        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            char roll[15], name[50], dept[50], pass[50];
            read_line("Enter ID", roll, sizeof(roll));
            //int roll = atoi(roll_s);
            read_line("Enter name", name, sizeof(name));
            read_line("Enter department", dept, sizeof(dept));
            read_line("Enter password", pass, sizeof(pass));
            add_student(roll, name, dept, pass);
        }
        else if (c == 2) list_students();
        else if (c == 3) {
		    char roll[50];
		    read_line("Enter ID to update", roll, sizeof(roll));
		    roll[strcspn(roll, "\r\n")] = '\0';   // IMPORTANT FIX
		
		    char name[50], dept[50], pass[50];
		    printf("\n--- Enter new details (leave empty to keep old) ---\n");
		
		    read_line("New Name", name, sizeof(name));
		    read_line("New Department", dept, sizeof(dept));
		    read_line("New Password", pass, sizeof(pass));
		
		    update_student(roll, name, dept, pass);
		}
        else if (c == 4) {
            char roll[15];
		    read_line("Enter ID to delete", roll, sizeof(roll));
		    //int roll = atoi(roll_s);
		    delete_student(roll);
        }
        else if (c == 5) {
		    char roll[50];
		    read_line("Enter ID to search", roll, sizeof(roll));
		    roll[strcspn(roll, "\r\n")] = '\0';
		
		    search_student(roll);
		}
		else if (c == 6) calculate_student_gpa();
        else if (c == 0){
        	system("cls");
        	break;	
		} 
        else{
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice.\n");
	        SetConsoleTextAttribute(h, 10);
		} 
    }
	
}

void manage_teachers(void) {
    char choice[10];
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    while (1) {
        printf("\n--- Teacher Management ---\n");
        printf("1. Add Teacher Record\n");
        printf("2. List Teachers Record\n");
        printf("3. Update Teacher Record\n");
        printf("4. Delete Teacher Record\n");
        printf("5. Search Teacher Record\n");
        printf("0. Back to Admin Menu\n");

        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            char id[10], name[50], dept[50], pass[50];
            read_line("Enter ID", id, sizeof(id));
            read_line("Enter Name", name, sizeof(name));
            read_line("Enter Department", dept, sizeof(dept));
            read_line("Enter Password", pass, sizeof(pass));
            add_teacher(id, name, dept, pass);
        }
        else if (c == 2) list_teachers();
        else if (c == 3) {
            char id[10], name[50], dept[50], pass[50];
            read_line("Enter ID to update", id, sizeof(id));
            printf("--- Enter new details (leave empty to keep old) ---\n");
            read_line("New Name", name, sizeof(name));
            read_line("New Department", dept, sizeof(dept));
            read_line("New Password", pass, sizeof(pass));
            update_teacher(id, name, dept, pass);
        }
        else if (c == 4) {
            char id[10];
            read_line("Enter ID to delete", id, sizeof(id));
            delete_teacher(id);
        }
        else if (c == 5) {
            char id[10];
            read_line("Enter ID to search", id, sizeof(id));
            search_teacher(id);
        }
        else if (c == 0){
        	system("cls");
			break;
    	}
        else{
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice.\n");
	        SetConsoleTextAttribute(h, 10);
		}
    }
}

void manage_courses(void) {
    char choice[10];
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    while (1) {
        printf("\n--- Course Management ---\n");
        printf("1. Add Course\n");
        printf("2. View Courses\n");
        printf("3. Update Course\n");
        printf("4. Delete Course\n");
        printf("5. Manage Student Course Assignments\n");
        printf("6. Manage Teacher Course Assignments\n");
        printf("0. Back to Admin Menu\n");
        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            char id[10], name[50], credits_s[10];
            int credits;
            read_line("Enter Course ID", id, sizeof(id));
            read_line("Enter Course Name", name, sizeof(name));
            read_line("Enter Credits", credits_s, sizeof(credits_s));
            credits = atoi(credits_s);

            add_course(id, name, credits);
        }
        else if (c == 2) list_courses();
        else if (c == 3){
        	char id[10], name[50], credits_s[10];
        	int credits;
            read_line("Enter Course ID to update", id, sizeof(id));
            printf("--- Enter new values (leave empty to keep old) ---\n");
            read_line("New Course Name", name, sizeof(name));
            read_line("New Credits", credits_s, sizeof(credits_s));
            credits = atoi(credits_s);
            update_course(id, name, credits);
		} 
		else if (c == 4){
        	char id[10];
	        read_line("Enter Course ID to delete", id, sizeof(id));
	        delete_course(id);
		} 
		else if (c == 5){
	        manage_student_course_assignments();
		} 
		else if (c == 6){
	        manage_teacher_course_assignments();
		} 
        else if (c == 0){
        	system("cls");
			break;
    	}
        else{
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice.\n");
	        SetConsoleTextAttribute(h, 10);
		}
    }
}

void manage_student_course_assignments(void) {
    char choice[10];
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    while (1) {
        printf("\n--- Student Course Assignments ---\n");
        printf("1. Assign Course to Student\n");
        printf("2. View Student Courses\n");
        printf("3. Remove Course From Student\n");
        printf("0. Back to Admin Menu\n");
        read_line("Enter choice", choice, sizeof(choice));
        if (!is_number(choice)) {
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice (numbers only).\n");
	        SetConsoleTextAttribute(h, 10);
	        continue;
	    }
        int c = atoi(choice);

        if (c == 1) {
            char studentID[50], courseID[50];

			read_line("Enter Student ID", studentID, sizeof(studentID));
			studentID[strcspn(studentID, "\n")] = '\0';
			
			read_line("Enter Course ID to assign", courseID, sizeof(courseID));
			courseID[strcspn(courseID, "\n")] = '\0';
			
			assign_course_to_student(studentID, courseID);
        }
        else if (c == 2) {
            char studentID[50];
			read_line("Enter Student ID", studentID, sizeof(studentID));
			studentID[strcspn(studentID, "\n")] = '\0';
			
			list_student_courses(studentID);
        }
        else if (c == 3) {
		    char studentID[50], courseID[50];
			read_line("Enter Student ID", studentID, sizeof(studentID));
			studentID[strcspn(studentID, "\n")] = '\0';
		    read_line("Enter Course ID to remove", courseID, sizeof(courseID));
		    remove_course_from_student(studentID, courseID);
		}
        else if (c == 0){	
        	system("cls");
			break;
		}
        else{
        	SetConsoleTextAttribute(h, 12);
	        printf("Invalid choice.\n");
	        SetConsoleTextAttribute(h, 10);
		} 
    }
}

void manage_teacher_course_assignments(void) {
    char choice_str[8];
    int choice;

    while (1) {
        printf("\n===== Manage Teacher Course Assignments =====\n");
        printf("1. Assign Course to Teacher\n");
        printf("2. View Teacher's Assigned Courses\n");
        printf("3. Remove a Course from Teacher\n");
        printf("0. Return to Admin Menu\n");

        read_line("Enter choice", choice_str, sizeof(choice_str));
        choice = atoi(choice_str);

        if (choice == 1) {
            char teacherID[20];
            char courseID[50];

            read_line("Enter Teacher ID", teacherID, sizeof(teacherID));
            read_line("Enter Course ID to Assign", courseID, sizeof(courseID));
			assign_course_to_teacher(teacherID, courseID);
        }

        else if (choice == 2) {
            char teacherID[20];

            read_line("Enter Teacher ID", teacherID, sizeof(teacherID));

            list_teacher_courses(teacherID);
        }

        else if (choice == 3) {
            char teacherID[20];
            char courseID[50];
            read_line("Enter Teacher ID", teacherID, sizeof(teacherID));
            read_line("Enter Course ID to Remove", courseID, sizeof(courseID));
            remove_course_from_teacher(teacherID, courseID);
        }

        else if (choice == 0){
        	system("cls");	
        	break;
		} 
        else {
            printf("Invalid choice. Try again.\n");
        }
    }
}



//helper functions:
void admin_banner(){
	printf("     _       _           _          __  __                   \n");
	printf("    / \\   __| |_ __ ___ (_)_ __    |  \\/  | ___ _ __  _   _  \n");
	printf("   / _ \\ / _` | '_ ` _ \\| | '_ \\   | |\\/| |/ _ \\ '_ \\| | | | \n");
	printf("  / ___ \\ (_| | | | | | | | | | |  | |  | |  __/ | | | |_| | \n");
	printf(" /_/   \\_\\__,_|_| |_| |_|_|_| |_|  |_|  |_|\\___|_| |_|\\__,_| \n");                                                 
}

//int get_course_credit(const char *courseID) {
//    FILE *f = fopen("data/courses.csv", "r");
//    if (!f) return 0;
//
//    char line[256];
//    while (fgets(line, sizeof(line), f)) {
//        char copy[256];
//        strcpy(copy, line);
//
//        char *cid = strtok(copy, ",");
//        strtok(NULL, ","); // course name
//        char *credit = strtok(NULL, ",\n");
//
//        if (cid && credit && strcmp(cid, courseID) == 0) {
//            fclose(f);
//            return atoi(credit);
//        }
//    }
//    fclose(f);
//    return 0;
//}

