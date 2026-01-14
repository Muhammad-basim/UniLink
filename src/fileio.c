#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // for isspace()
#include <windows.h>
#include "../include/fileio.h"
#include "../include/utils.h"

#define STUDENTS_FILE "data/students.csv"
#define TEACHERS_FILE "data/teachers.csv"
#define COURSES_FILE "data/courses.csv"

#define LINE_LEN 256
#define MAX_CREDITS 18
#define MAX_TEACHER_COURSES 4

//Student Management
int add_student(const char *id, const char *name, const char *dept, const char *pass) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(STUDENTS_FILE, "a");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Failed to open students file!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    fprintf(f, "%s,%s,%s,%s\n", id, name, dept, pass);
    fclose(f);
    printf("Student added successfully!\n");
    return 1;
}

void list_students(void) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[LINE_LEN];
    printf("\n--- Students List ---\n");
    printf("------------------------------------------------\n");
    printf("%-12s | %-20s | Department\n", "ID", "Name");
    printf("------------------------------------------------\n");
    while (fgets(line, sizeof(line), f)) {
		line[strcspn(line, "\r\n")] = 0;
		char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        if (!id || !name || !dept) continue;
        //printf("ID: %-5s | Name: %-20s | Dept: %-10s\n", id, name, dept);
        printf("%-5s | %-20s | %-10s\n", id, name, dept);
    }
    fclose(f);
}

int delete_student(const char *roll) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[LINE_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[LINE_LEN];
        strcpy(copy, line);
        char *token = strtok(copy, ",");
        char *file_roll = token;
        if (file_roll && strcmp(file_roll, roll) == 0) {
            found = 1;
            continue;   // skip this student
        }
        fputs(line, temp);
    }
    fclose(f);
    fclose(temp);
    remove(STUDENTS_FILE);
    rename("data/temp.csv", STUDENTS_FILE);
    if (found)
        printf("Student with ID %s deleted successfully!\n", roll);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Student not found!\n");
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}

int update_student(const char *id, const char *newName, const char *newDept, const char *newPass) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Error creating temporary file!\n");
        fclose(f);
        return 0;
    }
    char line[LINE_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[LINE_LEN];
        strcpy(copy, line);
        copy[strcspn(copy, "\r\n")] = '\0'; // remove trailing newline(s)
        char *file_id   = strtok(copy, ",");
        char *oldName   = strtok(NULL, ",");
        char *oldDept   = strtok(NULL, ",");
        char *oldPass   = strtok(NULL, ",");
        if (!file_id) {
            /* malformed line, write as-is */
            fprintf(temp, "%s", line);
            continue;
        }
        /* make sure tokens are not NULL before using them */
        if (!oldName) oldName = "";
        if (!oldDept) oldDept = "";
        if (!oldPass) oldPass = "";
        /* trim any trailing newline/carriage return from old tokens */
        oldName[strcspn(oldName, "\r\n")] = '\0';
        oldDept[strcspn(oldDept, "\r\n")] = '\0';
        oldPass[strcspn(oldPass, "\r\n")] = '\0';
        if (strcmp(file_id, id) == 0) {
            found = 1;
            const char *finalName = (newName && newName[0] != '\0') ? newName : oldName;
            const char *finalDept = (newDept && newDept[0] != '\0') ? newDept : oldDept;
            const char *finalPass = (newPass && newPass[0] != '\0') ? newPass : oldPass;
            fprintf(temp, "%s,%s,%s,%s\n", file_id, finalName, finalDept, finalPass);
        } else {
            /* write the original line exactly as it was read */
            fprintf(temp, "%s", line);
        }
    }
    fclose(f);
    fclose(temp);
    /* replace original file */
    remove(STUDENTS_FILE);
    rename("data/temp.csv", STUDENTS_FILE);
    if (found)
        printf("Student with ID %s updated successfully!\n", id);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Student not found!\n");
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}

void search_student(const char *id) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[LINE_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[LINE_LEN];
        strcpy(copy, line);
        char *file_roll = strtok(copy, ",");
        char *name      = strtok(NULL, ",");
        char *dept      = strtok(NULL, ",");
        char *pass      = strtok(NULL, ",");
        if (!file_roll) continue;
        if (!name)  name  = "";
        if (!dept)  dept  = "";
        if (!pass)  pass  = "";
        // clean newline
        pass[strcspn(pass, "\n")] = '\0';
        // compare student ID
        if (strcmp(file_roll, id) == 0) {
            found = 1;
            printf("\n--- Student Found ---\n");
	        printf("Student ID : %s\n", file_roll);
	        printf("Name       : %s\n", name);
	        printf("Department : %s\n", dept);
	        printf("Password   : %s\n", pass);
			//printf("ID: %s\n", file_roll);
			//printf("Name: %s\n", name);
			//printf("Department: %s\n", dept);
			//printf("Password: %s\n", pass);
            //printf("Student ID: %20s\nName: %19s\nDepartment: %s\nPassword: %5s\n", file_roll, name, dept,pass);
            break;
        }
    }
    if (!found) {
    	SetConsoleTextAttribute(h, 12);
		printf("Student with ID %s not found!\n", id);
		SetConsoleTextAttribute(h, 10);
    }
    fclose(f);
}


//Teacher Management
int add_teacher(const char *id, const char *name, const char *dept, const char *pass) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(TEACHERS_FILE, "a");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Failed to open teachers file!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    fprintf(f, "%s,%s,%s,%s\n", id, name, dept, pass);
    fclose(f);
    printf("Teacher added successfully!\n");
    return 1;
}

void list_teachers(void) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No teachers found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[256];
    printf("\n--- Teachers List ---\n");
    printf("------------------------------------\n");
    printf("%-5s | %-15s | Department\n", "ID", "Name");
    printf("------------------------------------\n");
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        if (!id) continue;
        if (!name) name = "";
        if (!dept) dept = "";
        if (!pass) pass = "";
        char *p = strchr(pass, '\n');
        if (p) *p = '\0';
        //printf("ID: %-5s | Name: %-15s | Dept: %-10s\n", id, name, dept);
        printf("%-5s | %-15s | %-10s\n", id, name, dept);
    }
    fclose(f);
}

int delete_teacher(const char *id) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No teachers found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (tid && strcmp(tid, id) == 0) {
            found = 1;
            continue;
        }
        fputs(line, temp);
    }

    fclose(f);
    fclose(temp);
    remove(TEACHERS_FILE);
    rename("data/temp.csv", TEACHERS_FILE);
    if (found)
        printf("Teacher with ID %s deleted successfully!\n", id);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher not found!\n");
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}

int update_teacher(const char *id, const char *newName, const char *newDept, const char *newPass) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No teachers found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        char *oldName = strtok(NULL, ",");
        char *oldDept = strtok(NULL, ",");
        char *oldPass = strtok(NULL, ",");
        if (!tid) continue;
        if (!oldName) oldName = "";
        if (!oldDept) oldDept = "";
        if (!oldPass) oldPass = "";
        char *p = strchr(oldPass, '\n');
        if (p) *p = '\0';
        if (strcmp(tid, id) == 0) {
            found = 1;
            const char *finalName = (strlen(newName) > 0) ? newName : oldName;
            const char *finalDept = (strlen(newDept) > 0) ? newDept : oldDept;
            const char *finalPass = (strlen(newPass) > 0) ? newPass : oldPass;
            fprintf(temp, "%s,%s,%s,%s\n", id, finalName, finalDept, finalPass);
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    remove(TEACHERS_FILE);
    rename("data/temp.csv", TEACHERS_FILE);
    if (found)
        printf("Teacher with ID %s updated successfully!\n", id);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher not found!\n");
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}

void search_teacher(const char *id) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No teachers found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        if (!tid) continue;
        if (!name) name = "";
        if (!dept) dept = "";
        if (!pass) pass = "";
        char *p = strchr(pass, '\n');
        if (p) *p = '\0';
        if (strcmp(tid, id) == 0) {
            found = 1;
            printf("\n--- Teacher Found ---\n");
            printf("ID: %12s\nName: %16s\nDepartment: %s\n", tid, name, dept);
            break;
        }
    }
    if (!found){
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher with ID %s not found!\n", id);
		SetConsoleTextAttribute(h, 10);
	} 
    fclose(f);
}

//Course Management
int add_course(const char *courseID, const char *name, int credits) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(COURSES_FILE, "a");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Failed to open courses file!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    fprintf(f, "%s,%s,%d\n", courseID, name, credits);
    fclose(f);
    printf("Course with course ID %s added successfully!\n", courseID);
    return 1;
}

void list_courses(void) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No courses found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[256];
    printf("\n--- Courses List ---\n");
    printf("-----------------------------------------------------\n");
    printf("%-10s | %-30s | Credits\n", "CourseID", "Course Name");
    printf("-----------------------------------------------------\n");
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *credits_str = strtok(NULL, ",");
        if (!id || !name || !credits_str) continue;
        char *p = strchr(credits_str, '\n');
        if (p) *p = '\0';
        printf("%-10s | %-30s | %s\n", id, name, credits_str);
    }
    fclose(f);
}

int update_course(const char *courseID, const char *newName, int *newCredits) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No Courses found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *credits_str = strtok(NULL, ",");
        if (!id || !name || !credits_str) continue;
        char *p = strchr(credits_str, '\n');
        if (p) *p = '\0';
        if (strcmp(id, courseID) == 0) {
            found = 1;
            const char *finalName = (strlen(newName) > 0) ? newName : name;
            int finalCredits = (newCredits > 0) ? newCredits : atoi(credits_str);
            fprintf(temp, "%s,%s,%d\n", id, finalName, finalCredits);
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    remove(COURSES_FILE);
    rename("data/temp.csv", COURSES_FILE);
    if (found)
        printf("Course with course ID %s updated successfully!\n", courseID);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Course with course ID %s not found!\n", courseID);
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}

int delete_course(const char *courseID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No courses found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        if (!id) continue;
        if (strcmp(id, courseID) == 0) {
            found = 1;
            continue; // skip this course (delete)
        }
        fputs(line, temp);
    }
    fclose(f);
    fclose(temp);
    remove(COURSES_FILE);
    rename("data/temp.csv", COURSES_FILE);
    if (found)
        printf("Course with course ID %s deleted successfully!\n", courseID);
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Course with course ID %s not found!\n", courseID);
		SetConsoleTextAttribute(h, 10);
	}
    return found;
}


//Student Course Management
int assign_course_to_student(const char *studentID, const char *courseID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char name[50]="", dept[50]="";
    // --- 1. Get student info
    if (!get_student_info(studentID, name, dept)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Student '%s' not found!\n", studentID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    // --- 2. Check course exists
    if (!course_exists(courseID)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Course '%s' does not exist!\n", courseID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    // --- 3. Prevent duplicate course
    if (student_has_course(studentID, courseID)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Course '%s' already assigned to this student.\n", courseID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    // --- 4. Check credit limit
    int currentCredits = get_student_total_credits(studentID);
    int newCredits = get_course_credits(courseID);
    if (currentCredits + newCredits > MAX_CREDITS) {
    	SetConsoleTextAttribute(h, 12);
		printf("\nCannot assign course '%s' to student '%s'!\n", courseID, studentID);
		SetConsoleTextAttribute(h, 10);
		printf("Current Credits : %d\n", currentCredits);
	    printf("Course(%s) Credits : %d\n",courseID, newCredits);
	    printf("Maximum Allowed : %d\n", MAX_CREDITS);
        return 0;
    }
    // --- 5. Update CSV
    FILE *f = fopen("data/student_courses.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) { if(f) fclose(f); printf("Temp file error\n"); return 0; }
    int studentFound = 0;
    char line[512];
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char copy[512]; strcpy(copy, line);
            char *id = strtok(copy, ",\n");
            if (!id) { fputs(line, temp); continue; }
            trim(id);
            if (strcmp(id, studentID) == 0) {
                studentFound = 1;
                line[strcspn(line, "\n")] = '\0';
                fprintf(temp, "%s,%s\n", line, courseID);
                printf("Course '%s' assigned successfully!\n", courseID);
            } else {
                fputs(line, temp);
            }
        }
        fclose(f);
    }
    // Student not in CSV yet
    if (!studentFound) {
        fprintf(temp, "%s,%s,%s,%s\n", studentID, name, dept, courseID);
        printf("Course '%s' assigned successfully!\n", courseID);
    }
    fclose(temp);
    remove("data/student_courses.csv");
    rename("data/temp.csv", "data/student_courses.csv");
    return 1;
}

void list_student_courses(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No course assignments found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        /* tokenize once */
        char *tokens[50];
        int count = 0;
        char *tok = strtok(copy, ",");
        while (tok && count < 50) {
            tok[strcspn(tok, "\n")] = '\0';  // remove newline
            tokens[count++] = tok;
            tok = strtok(NULL, ",");
        }
        if (count < 3) continue;  // malformed line
        /* check if this is the student we want */
        if (strcmp(tokens[0], studentID) != 0)
            continue;
        found = 1;
        printf("\n--- Student Courses ---\n");
        printf("Student ID : %s\n", tokens[0]);
        printf("Name       : %s\n", tokens[1]);
        printf("Department : %s\n", tokens[2]);
        printf("Assigned Courses:\n");
        int totalCredits = 0;
        if (count == 3) {
            printf("  None\n");
        } else {
        	int i;
            for (i = 3; i < count; i++) {
                int credits = get_course_credits(tokens[i]);
                if (credits > 0) {
                    printf("  - %s (%d credits)\n", tokens[i], credits);
                    totalCredits += credits;
                } else {
                    printf("  - %s (credits unknown)\n", tokens[i]);
                }
            }
        }
        printf("\nTotal Credits: %d / %d\n", totalCredits, MAX_CREDITS);
        break;  // found the student, no need to read more
    }
    if (!found){
    	SetConsoleTextAttribute(h, 12);
		printf("No courses assigned to student ID '%s'.\n", studentID);
		SetConsoleTextAttribute(h, 10);
	}
    fclose(f);
}

int remove_course_from_student(const char *studentID, const char *courseID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No course assignments file found!\n");
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[512];
    int found_student = 0;
    int removed_course = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        if (!id) { fputs(line, temp); continue; }
        if (strcmp(id, studentID) != 0) {
            // Not the student we want, copy as is
            fputs(line, temp);
            continue;
        }
        found_student = 1;
        // Split original line into tokens
        char *tokens[100];
        int token_count = 0;
        char *tok = strtok(line, ",\n");
        while (tok) {
            tokens[token_count++] = tok;
            tok = strtok(NULL, ",\n");
        }
        // tokens[0]=ID, tokens[1]=name, tokens[2]=dept, tokens[3..]=courses
        int i, course_found = 0;
        for (i = 3; i < token_count; i++) {
            if (strcmp(tokens[i], courseID) == 0) {
                course_found = 1; // skip this course
            }
        }
        if (!course_found) {
        	SetConsoleTextAttribute(h, 12);
			printf("Course '%s' not found for this student.\n", courseID);
			SetConsoleTextAttribute(h, 10);
            fputs(line, temp);
            continue;
        }
        removed_course = 1;
        // Rebuild line without removed course
        fprintf(temp, "%s,%s,%s", tokens[0], tokens[1], tokens[2]);
        for (i = 3; i < token_count; i++) {
            if (strcmp(tokens[i], courseID) != 0) {
                fprintf(temp, ",%s", tokens[i]);
            }
        }
        fprintf(temp, "\n");
    }
    fclose(f);
    fclose(temp);
    // Replace original file
    remove("data/student_courses.csv");
    rename("data/temp.csv", "data/student_courses.csv");
    if (!found_student) {
    	SetConsoleTextAttribute(h, 12);
		printf("Student with ID %s not found in assignment file.\n", studentID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    if (!removed_course) {
        return 0;
    }
    printf("Course '%s' removed successfully from student %s!\n", courseID, studentID);
    return 1;
}

void calculate_student_gpa() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char studentID[20];
    read_line("Enter Student ID", studentID, sizeof(studentID));
    FILE *fm = fopen("data/course_marks.csv", "r");
    if (!fm) {
    	SetConsoleTextAttribute(h, 12);
		printf("Marks file not found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    float totalWeighted = 0.0;
    float totalCredits = 0.0;
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fm)) {
        char copy[256];
        strcpy(copy, line);
        char *courseID = strtok(copy, ",");
        char *sid = strtok(NULL, ",");
        char *marksStr = strtok(NULL, ",\n");
        if (!courseID || !sid || !marksStr)
            continue;
        trim(courseID);
		trim(sid);
		trim(marksStr);
        if (strcmp(sid, studentID) == 0) {
            float marks = atof(marksStr);
            float gp = 0.00;
//            float gp = marks_to_gp(marks);
            if (marks >= 85) gp = 4.0f;
		    else if (marks >= 80) gp = 3.7f;
		    else if (marks >= 75) gp = 3.3f;
		    else if (marks >= 70) gp = 3.0f;
		    else if (marks >= 65) gp = 2.7f;
		    else if (marks >= 60) gp = 2.3f;
		    else if (marks >= 50) gp = 2.0f;
		    else gp = 0.0f;
            
            int credit = get_course_credits(courseID);
            if (credit <= 0) {
            	SetConsoleTextAttribute(h, 12);
				printf("Warning: Credit hours not found for %s\n", courseID);
				SetConsoleTextAttribute(h, 10);
			    continue;
			}
			else{
				printf("\nCourseID: '%s'", courseID);
				printf("\nMarks   : %.2f", marks);
				printf("\nCredits : %d\n", credit);
				totalWeighted += gp * credit;
                totalCredits += credit;
                found = 1;
			}
        }
    }
    fclose(fm);
    if (!found || totalCredits == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No complete data found for GPA calculation.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    float gpa = totalWeighted / totalCredits;
    FILE *fg = fopen("data/student_gpa.csv", "w");
    if (!fg) {
    	SetConsoleTextAttribute(h, 12);
		printf("Unable to save GPA.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    fprintf(fg, "%s,%.2f\n", studentID, gpa);
    fclose(fg);
    printf("\nGPA Calculated Successfully\n");
    printf("Student ID: %s\n", studentID);
    printf("GPA: %.2f\n", gpa);
}


//Teacher Course Management
int get_teacher_info(const char *id, char *name, char *dept) {
    FILE *f = fopen("data/teachers.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        // Remove \n and \r
        line[strcspn(line, "\r\n")] = 0;
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        char *tname = strtok(NULL, ",");
        char *tdept = strtok(NULL, ",");  // we only need first 3
        if (!tid || !tname || !tdept)
            continue;
        // Compare cleaned ID
        if (strcmp(tid, id) == 0) {
            strcpy(name, tname);
            strcpy(dept, tdept);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int assign_course_to_teacher(const char *teacherID, const char *courseID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char name[50], dept[50];
    // 1. Validate teacher
    if (!get_teacher_info(teacherID, name, dept)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher '%s' not found!\n", teacherID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    // 2. Validate course
    if (!course_exists(courseID)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Course '%s' does not exist!\n", courseID);
		SetConsoleTextAttribute(h, 10);
        return 0;
    }
    // 3. Enforce course limit
    int currentCount = get_teacher_course_count(teacherID);
    if (currentCount >= MAX_TEACHER_COURSES) {
    	SetConsoleTextAttribute(h, 12);
		printf("Cannot assign course.\n");
		SetConsoleTextAttribute(h, 10);
        printf("Teacher already has %d out of %d courses.\n",
               currentCount, MAX_TEACHER_COURSES);
        return 0;
    }
    FILE *f = fopen("data/teacher_courses.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) { if (f) fclose(f); return 0; }
    char line[512];
    int found = 0;
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char original[512];
            strcpy(original, line);
            char copy[512];
            strcpy(copy, line);
            char *id = strtok(copy, ",");
            if (!id) {
                fputs(original, temp);
                continue;
            }
            if (strcmp(id, teacherID) == 0) {
                found = 1;
                // check duplicate
                char *token;
                int duplicate = 0;
                strtok(NULL, ","); // name
                strtok(NULL, ","); // dept
                while ((token = strtok(NULL, ","))) {
                    if (strcmp(token, courseID) == 0) {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate) {
                	SetConsoleTextAttribute(h, 12);
					printf("Course '%s' already assigned to this teacher.\n", courseID);
					SetConsoleTextAttribute(h, 10);
                    fputs(original, temp);
                } else {
                    original[strcspn(original, "\n")] = '\0';
                    fprintf(temp, "%s,%s\n", original, courseID);
                    printf("Course '%s' assigned to teacher successfully!\n", courseID);
                }
            } else {
                fputs(original, temp);
            }
        }
        fclose(f);
    }
    // teacher not in file yet
    if (!found) {
        fprintf(temp, "%s,%s,%s,%s\n", teacherID, name, dept, courseID);
        printf("Course '%s' assigned to teacher successfully!\n", courseID);
    }
    fclose(temp);
    remove("data/teacher_courses.csv");
    rename("data/temp.csv", "data/teacher_courses.csv");
    return 1;
}

void list_teacher_courses(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("No teacher course records found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tokens[20];
        int count = 0;
        char *tok = strtok(copy, ",\n");
        while (tok && count < 20) {
            tokens[count++] = tok;
            tok = strtok(NULL, ",\n");
        }
        if (count < 3) continue;
        if (strcmp(tokens[0], teacherID) != 0) continue;
        found = 1;
        printf("\n--- Teacher Courses ---\n");
        printf("Teacher ID : %s\n", tokens[0]);
        printf("Name       : %s\n", tokens[1]);
        printf("Department : %s\n", tokens[2]);
        printf("Assigned Courses:\n");
        if (count == 3) {
            printf("  None\n");
        } else {
        	int i;
            for (i = 3; i < count; i++)
                printf("  - %s\n", tokens[i]);
        }
        break;
    }
    if (!found){
    	SetConsoleTextAttribute(h, 12);
		printf("No courses assigned to teacher '%s'.\n", teacherID);
		SetConsoleTextAttribute(h, 10);
	}
    fclose(f);
}

int remove_course_from_teacher(const char *teacherID, const char *courseID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) return 0;
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) { fclose(f); return 0; }
    char line[512];
    int found = 0, removed = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tokens[20];
        int count = 0;
        char *tok = strtok(copy, ",\n");
        while (tok && count < 20) {
            tokens[count++] = tok;
            tok = strtok(NULL, ",\n");
        }
        if (count < 3 || strcmp(tokens[0], teacherID) != 0) {
            fputs(line, temp);
            continue;
        }
        found = 1;
        fprintf(temp, "%s,%s,%s", tokens[0], tokens[1], tokens[2]);
		int i;
        for (i = 3; i < count; i++) {
            if (strcmp(tokens[i], courseID) != 0) {
                fprintf(temp, ",%s", tokens[i]);
            } else {
                removed = 1;
            }
        }
        fprintf(temp, "\n");
    }
    fclose(f);
    fclose(temp);
    remove("data/teacher_courses.csv");
    rename("data/temp.csv", "data/teacher_courses.csv");
    if (!found){
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher '%s' not found.\n",teacherID);
		SetConsoleTextAttribute(h, 10);
	} 
    else if (!removed){
    	SetConsoleTextAttribute(h, 12);
		printf("Course '%s' not found for this teacher.\n",courseID);
		SetConsoleTextAttribute(h, 10);
	} 
    else printf("Course '%s' removed successfully.\n", courseID);
    return removed;
}


//Helper Functions:
//* helper: trim leading + trailing whitespace (modifies string in-place) */
void trim(char *s) {
    if (!s) return;
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start)+1);
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) {
        s[len-1] = '\0';
        len--;
    }
}

//void trim(char *str) {
//    // Trim leading spaces
//    int index = 0;
//    while (str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
//        index++;
//
//    if (index != 0)
//        memmove(str, str + index, strlen(str + index) + 1);
//
//    // Trim trailing spaces
//    int end = strlen(str) - 1;
//    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
//        str[end] = '\0';
//        end--;
//    }
//}


int course_exists(const char *courseID) {
    FILE *f = fopen("data/courses.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *id = strtok(line, ",\n");
        if (id) trim(id);
        if (id && strcmp(id, courseID) == 0) { fclose(f); return 1; }
    }
    fclose(f);
    return 0;
}

//int get_course_credits(const char *courseID) {
//    FILE *f = fopen("data/courses.csv", "r");
//    if (!f) return -1;
//    char line[256];
//    while (fgets(line, sizeof(line), f)) {
//        char *id = strtok(line, ",\n");
//        char *name = strtok(NULL, ",\n");
//        char *credits = strtok(NULL, ",\n");
//        if (!id || !credits) continue;
//        trim(id); trim(credits);
//        if (strcmp(id, courseID) == 0) {
//            fclose(f);
//            return atoi(credits);
//        }
//    }
//    fclose(f);
//    return -1;
//}
//float marks_to_gp(float marks)
//{
//	printf("marks after function: '%.2f'\n", marks);
//    if (marks >= 85) return 4.0f;
//    else if (marks >= 80) return 3.7f;
//    else if (marks >= 75) return 3.3f;
//    else if (marks >= 70) return 3.0f;
//    else if (marks >= 65) return 2.7f;
//    else if (marks >= 60) return 2.3f;
//    else if (marks >= 50) return 2.0f;
//    else return 0.0f;
//}


int get_course_credits(const char *courseID) {
    FILE *f = fopen("data/courses.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *cid = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *creditStr = strtok(NULL, ",\n");
        if (!cid || !creditStr)
            continue;
        trim(cid);
        trim(creditStr);
        /* Skip header */
        if (strcmp(cid, "CourseID") == 0)
            continue;
        if (strcmp(cid, courseID) == 0) {
            fclose(f);
            return atoi(creditStr);
        }
    }
    fclose(f);
    return 0;
}

int get_student_info(const char *studentID, char *name, char *dept) {
    FILE *f = fopen("data/students.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *id = strtok(line, ",\n");
        char *n  = strtok(NULL, ",\n");
        char *d  = strtok(NULL, ",\n");
        if (!id) continue;
        trim(id); if (strcmp(id, studentID) == 0) {
            if (n) { strncpy(name, n, 49); name[49]='\0'; trim(name); }
            if (d) { strncpy(dept, d, 49); dept[49]='\0'; trim(dept); }
            fclose(f); return 1;
        }
    }
    fclose(f);
    return 0;
}

int get_student_total_credits(const char *studentID) {
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) return 0;
    char line[512];
    int total = 0;
    while (fgets(line, sizeof(line), f)) {
        char lineCopy[512];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        char *tokens[100];
        int fieldCount = 0;
        char *token = strtok(lineCopy, ",");
        while (token && fieldCount < 100) {
            trim(token);
            tokens[fieldCount++] = token;
            token = strtok(NULL, ",");
        }
        if (fieldCount < 4) continue; // no courses

        if (strcmp(tokens[0], studentID) == 0) {
            // sum credits of courses starting from index 3
            int i;
            for (i = 3; i < fieldCount; i++) {
                int credits = get_course_credits(tokens[i]);
                if (credits > 0) total += credits;
            }
            break;
        }
    }
    fclose(f);
    return total;
}

int student_has_course(const char *studentID, const char *courseID) {
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) return 0;
    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512]; strcpy(copy, line);
        char *id = strtok(copy, ",\n");
        if (!id) continue;
        trim(id);
        if (strcmp(id, studentID) == 0) {
            strtok(NULL, ",\n"); // skip name
            strtok(NULL, ",\n"); // skip dept
            char *token = NULL;
            while ((token = strtok(NULL, ",\n")) != NULL) {
                trim(token);
                if (strcmp(token, courseID) == 0) { found = 1; break; }
            }
            break;
        }
    }
    fclose(f);
    return found;
}

int get_teacher_course_count(const char *teacherID) {
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) return 0;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);

        char *id = strtok(copy, ",");
        if (!id) continue;

        if (strcmp(id, teacherID) == 0) {
            int count = 0;
            strtok(NULL, ","); // name
            strtok(NULL, ","); // dept

            while (strtok(NULL, ",")) count++;
            fclose(f);
            return count;
        }
    }

    fclose(f);
    return 0;
}



