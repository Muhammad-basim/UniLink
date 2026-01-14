#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/teacher.h"
#include "../include/utils.h"
#include "../include/teacher_quiz.h"

#define COURSE_MARKS_FILE "data/course_marks.csv"

void view_my_assigned_courses(const char *teacherID) {
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) {
        printf("Teacher courses file not found!\n");
        return;
    }
    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        /* tokenize line */
        char *tokens[50];
        int count = 0;
        char *tok = strtok(copy, ",");
        while (tok && count < 50) {
            tok[strcspn(tok, "\n")] = '\0';  // remove newline
            trim(tok);                       // IMPORTANT
            tokens[count++] = tok;
            tok = strtok(NULL, ",");
        }
        if (count < 3)
            continue;
        /* match teacher ID */
        if (strcmp(tokens[0], teacherID) != 0)
            continue;
        found = 1;
        printf("\n--- My Assigned Courses ---\n");
        printf("Teacher ID : %s\n", tokens[0]);
        printf("Name       : %s\n", tokens[1]);
        printf("Department : %s\n", tokens[2]);
        printf("Courses:\n");
        if (count == 3) {
            printf("  None\n");
        } else {
            int i;
            for (i = 3; i < count; i++) {
                printf("  - %s\n", tokens[i]);
            }
        }
        printf("Total Courses Assigned: %d / %d\n",
               count > 3 ? count - 3 : 0,
               MAX_TEACHER_COURSES);
        break;
    }
    fclose(f);
    if (!found) {
        printf("No courses assigned to teacher ID '%s'.\n", teacherID);
    }
}

void view_students_in_course(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher courses file not found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[512];
    char teacherCourses[4][20]; // assume max 10 courses per teacher
    int courseCount = 0;
    // --- Step 1: List teacher's courses ---
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (!tid) continue;
        trim(tid);
        if (strcmp(tid, teacherID) != 0) continue;
        // Skip name and department
        strtok(NULL, ",");
        strtok(NULL, ",");
        char *tok;
        while ((tok = strtok(NULL, ",\n")) != NULL && courseCount < 10) {
            trim(tok);
            strcpy(teacherCourses[courseCount++], tok);
        }
        break;
    }
    fclose(f);
    if (courseCount == 0) {
        printf("No courses assigned to teacher ID '%s'.\n", teacherID);
        return;
    }
    printf("\n--- My Courses ---\n");
    int i;
    for (i = 0; i < courseCount; i++) {
        printf("%d. %s\n", i + 1, teacherCourses[i]);
    }
    char choice[10];
	read_line("Enter the number of the course to view students", choice, sizeof(choice));
	if (!is_number(choice)) {
		SetConsoleTextAttribute(h, 12);
		printf("Invalid choice (numbers only).\n");
		SetConsoleTextAttribute(h, 10);
	    return;
	}
	int c = atoi(choice);
	int numCourses = 0;
	for (i = 1; i <= 4; i++) {
	    if (strlen(teacherCourses[i]) > 0) numCourses++;
	}
	// Check if number is within the available courses
	if (c < 1 || c > numCourses) {  // numCourses = total courses in teacherCourses array
		SetConsoleTextAttribute(h, 12);
		printf("Invalid choice: please select only avaiable course.\n");
		SetConsoleTextAttribute(h, 10);
    	return;
	}
    char *selectedCourse = teacherCourses[c - 1];
    // --- Step 3: List students in the selected course ---
    FILE *sf = fopen("data/student_courses.csv", "r");
    if (!sf) {
    	SetConsoleTextAttribute(h, 12);
		printf("Students file not found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    int studentFound = 0;
    printf("\n--- Students in %s ---\n", selectedCourse);
    printf("------------------------------------------------\n");
    printf("%-12s | %-20s | Department\n", "ID", "Name");
    printf("------------------------------------------------\n");
    while (fgets(line, sizeof(line), sf)) {
        char copy[512]; 
        strcpy(copy, line);
        char *sid = strtok(copy, ",");
        char *sname = strtok(NULL, ",");
        char *sdept = strtok(NULL, ",");
        if (!sid || !sname || !sdept) continue;
        char *tok;
        while ((tok = strtok(NULL, ",\n")) != NULL) {
            trim(tok);
            if (strcmp(tok, selectedCourse) == 0) {
                studentFound = 1;
                //printf("ID: %s | Name: %s | Department: %s\n", sid, sname, sdept);
                printf("%-5s | %-20s | %-10s\n", sid, sname, sdept);
                break;
            }
        }
    }
    fclose(sf);
    if (!studentFound) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students enrolled in %s.\n", selectedCourse);
		SetConsoleTextAttribute(h, 10);
    }
}

void change_teacher_password(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char currentPass[50], newPass[50], confirmPass[50];
    char line[512];
    int found = 0;
    read_password(currentPass, sizeof(currentPass), "Enter current password");
    FILE *f = fopen("data/teachers.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Teachers file not found!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        fclose(f);
        SetConsoleTextAttribute(h, 12);
		printf("Failed to create temp file!\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        char *tname = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *tpass = strtok(NULL, ",\n");
        if (!tid || !tname || !tpass) {
            fputs(line, temp);
            continue;
        }
        if (strcmp(tid, teacherID) == 0) {
            found = 1;
            if (strcmp(currentPass, tpass) != 0) {
            	SetConsoleTextAttribute(h, 12);
				printf("Incorrect current password!\n");
				SetConsoleTextAttribute(h, 10);
                fputs(line, temp); // keep old password
                continue;
            }
            // Get new password
            while (1) {
                read_password(newPass, sizeof(newPass), "Enter new password");
                read_password(confirmPass, sizeof(confirmPass), "Confirm new password");
                if (strcmp(newPass, confirmPass) != 0) {
                	SetConsoleTextAttribute(h, 12);
					printf("Passwords do not match! Try again.\n");
					SetConsoleTextAttribute(h, 10);
                } else {
                    break;
                }
            }
            // Write updated line
            fprintf(temp, "%s,%s,%s,%s\n", tid, tname, dept, newPass);
            printf("Password updated successfully!\n");
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);

    if (!found) {
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher ID not found!\n");
		SetConsoleTextAttribute(h, 10);
        remove("data/temp.csv");
        return;
    }
    remove("data/teachers.csv");
    rename("data/temp.csv", "data/teachers.csv");
}

void upload_marks(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    // 1. Get teacher's assigned courses
    char teacherCourses[4][20];
    int numCourses = 0;
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Teacher courses file not found!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512]; strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (!tid) continue;
        trim(tid);
        if (strcmp(tid, teacherID) != 0) continue;
        strtok(NULL, ","); strtok(NULL, ","); // skip name & dept
        char *tok = NULL; int i = 0;
        while ((tok = strtok(NULL, ",\n")) != NULL && i < 4) {
            trim(tok);
            strncpy(teacherCourses[i++], tok, sizeof(teacherCourses[i-1])-1);
        }
        numCourses = i;
        break;
    }
    fclose(f);
    if (numCourses == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No courses assigned.\n");
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    // 2. Select course
    printf("\n--- Assigned Courses ---\n");
    int i;
    for (i = 0; i < numCourses; i++)
        printf("%d. %s\n", i+1, teacherCourses[i]);
    char choice[10];
    read_line("Select course number to upload marks", choice, sizeof(choice));
    if (!is_number(choice)) { 
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    int c = atoi(choice);
    if (c < 1 || c > numCourses) { 
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid course choice!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    char *selectedCourse = teacherCourses[c-1];
    // 3. Load all students enrolled in that course
    FILE *sf = fopen("data/student_courses.csv", "r");
    if (!sf) {
    	SetConsoleTextAttribute(h, 12);
		printf("Student courses file not found!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    char students[100][2][50];
    int studentCount = 0;
    while (fgets(line, sizeof(line), sf)) {
        char copy[512]; strcpy(copy, line);
        char *sid = strtok(copy, ",");
        char *sname = strtok(NULL, ",");
        strtok(NULL, ","); // skip dept
        char *tok = NULL;
        int enrolled = 0;
        while ((tok = strtok(NULL, ",\n")) != NULL) {
            trim(tok);
            if (strcmp(tok, selectedCourse) == 0) { enrolled = 1; break; }
        }
        if (enrolled && studentCount < 100) {
            trim(sid); trim(sname);
            strncpy(students[studentCount][0], sid, sizeof(students[0][0])-1);
            strncpy(students[studentCount][1], sname, sizeof(students[0][1])-1);
            studentCount++;
        }
    }
    fclose(sf);
    if (studentCount == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students enrolled in %s.\n", selectedCourse); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    // 4. Display all students
    printf("\n--- Students in %s ---\n", selectedCourse);
    printf("------------------------------------------------\n");
    printf("%-15s %-25s %-10s\n", "Student ID", "Name", "Marks");
    printf("------------------------------------------------\n");
    
    // --- Load marks ---
    int marks[100]; // store marks per student
    for (i = 0; i < studentCount; i++) marks[i] = -1; // -1 = not yet assigned
    FILE *mf = fopen("data/course_marks.csv", "r");
    if (mf) {
        while (fgets(line, sizeof(line), mf)) {
            char copy[256]; strcpy(copy, line);
            char *cid = strtok(copy, ",");
            char *sid = strtok(NULL, ",");
            char *markStr = strtok(NULL, ",\n");
            if (!cid || !sid || !markStr) continue;
            trim(cid); trim(sid); trim(markStr);
            if (strcmp(cid, selectedCourse) != 0) continue;
            for (i = 0; i < studentCount; i++) {
                if (strcmp(sid, students[i][0]) == 0) {
                    marks[i] = atoi(markStr);
                    break;
                }
            }
        }
        fclose(mf);
    }
    for (i = 0; i < studentCount; i++)
        //printf("%d. %s (%s)\n", i+1, students[i][1], students[i][0]);
        if (marks[i] >= 0)
            printf("%-15s %-25s %-10d\n", students[i][0], students[i][1], marks[i]);
        else
            printf("%-15s %-25s %-10s\n", students[i][0], students[i][1], "N/A");
    // 5. Loop to upload marks by student ID with update capability
    while (1) {
        char studentID[20];
        read_line("\nEnter Student ID to upload marks (or 0 to finish)", studentID, sizeof(studentID));
        if (strcmp(studentID, "0") == 0) break;
        int found = 0;
        for (i = 0; i < studentCount; i++) {
            if (strcmp(studentID, students[i][0]) == 0) {
                found = 1;
                char markInput[10];
                char prompt[100];
                snprintf(prompt, sizeof(prompt), "Enter marks for %s (%s)", students[i][1], students[i][0]);
                read_line(prompt, markInput, sizeof(markInput));
                // --- Read existing marks into memory ---
                FILE *mf = fopen("data/course_marks.csv", "r");
                FILE *temp = fopen("data/temp_marks.csv", "w");
                if (!temp) { printf("Failed to create temp file.\n"); if(mf) fclose(mf); return; }
                int updated = 0;
                if (mf) {
                    char markLine[256];
                    while (fgets(markLine, sizeof(markLine), mf)) {
                        char markCopy[256]; strcpy(markCopy, markLine);
                        char *cid = strtok(markCopy, ",");
                        char *sid = strtok(NULL, ",");
                        strtok(NULL, ","); // skip previous marks
                        if (strcmp(cid, selectedCourse) == 0 && strcmp(sid, studentID) == 0) {
                            // update existing mark
                            fprintf(temp, "%s,%s,%s\n", selectedCourse, studentID, markInput);
                            updated = 1;
                        } else {
                            fputs(markLine, temp);
                        }
                    }
                    fclose(mf);
                }
                // if not updated, append new record
                if (!updated)
                    fprintf(temp, "%s,%s,%s\n", selectedCourse, studentID, markInput);
                fclose(temp);
                remove("data/course_marks.csv");
                rename("data/temp_marks.csv", "data/course_marks.csv");
                printf("Marks saved for %s.\n", students[i][1]);
                break;
            }
        }
        if (!found){
        	SetConsoleTextAttribute(h, 12);
			printf("Invalid Student ID!\n");
			SetConsoleTextAttribute(h, 10);
		} 
    }
    printf("Finished uploading marks for course %s.\n", selectedCourse);
}

void view_course_student_marks(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char teacherCourses[4][20];
    int numCourses = 0;
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) { printf("Teacher courses file not found!\n"); return; }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512]; strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (!tid) continue;
        trim(tid);
        if (strcmp(tid, teacherID) != 0) continue;
        strtok(NULL, ","); strtok(NULL, ","); // skip name & dept

        char *tok = NULL; int i = 0;
        while ((tok = strtok(NULL, ",\n")) != NULL && i < 4) {
            trim(tok);
            strncpy(teacherCourses[i++], tok, sizeof(teacherCourses[i-1])-1);
        }
        numCourses = i;
        break;
    }
    fclose(f);

    if (numCourses == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No courses assigned.\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}

    // --- Select course ---
    printf("\n--- Assigned Courses ---\n");
    int i;
    for (i = 0; i < numCourses; i++)
        printf("%d. %s\n", i+1, teacherCourses[i]);

    char choice[10];
    read_line("Select course number to view student marks", choice, sizeof(choice));
    if (!is_number(choice)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}
    int c = atoi(choice);
    if (c < 1 || c > numCourses) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid course choice!\n"); 
		SetConsoleTextAttribute(h, 10);
		return; 
	}

    char *selectedCourse = teacherCourses[c-1];

    // --- Load students enrolled in course ---
    char students[100][2][50]; // 0=ID, 1=Name
    int studentCount = 0;
    FILE *sf = fopen("data/student_courses.csv", "r");
    if (!sf) { printf("Student courses file not found!\n"); return; }

    while (fgets(line, sizeof(line), sf)) {
        char copy[512]; strcpy(copy, line);
        char *sid = strtok(copy, ",");
        char *sname = strtok(NULL, ",");
        strtok(NULL, ","); // skip dept

        char *tok = NULL;
        int enrolled = 0;
        while ((tok = strtok(NULL, ",\n")) != NULL) {
            trim(tok);
            if (strcmp(tok, selectedCourse) == 0) { enrolled = 1; break; }
        }

        if (enrolled && studentCount < 100) {
            trim(sid); trim(sname);
            strncpy(students[studentCount][0], sid, sizeof(students[0][0])-1);
            strncpy(students[studentCount][1], sname, sizeof(students[0][1])-1);
            studentCount++;
        }
    }
    fclose(sf);

    if (studentCount == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No students enrolled in %s.\n", selectedCourse);
		SetConsoleTextAttribute(h, 10);
        return;
    }

    // --- Load marks ---
    int marks[100]; // store marks per student
    for (i = 0; i < studentCount; i++) marks[i] = -1; // -1 = not yet assigned

    FILE *mf = fopen("data/course_marks.csv", "r");
    if (mf) {
        while (fgets(line, sizeof(line), mf)) {
            char copy[256]; strcpy(copy, line);
            char *cid = strtok(copy, ",");
            char *sid = strtok(NULL, ",");
            char *markStr = strtok(NULL, ",\n");
            if (!cid || !sid || !markStr) continue;
            trim(cid); trim(sid); trim(markStr);
            if (strcmp(cid, selectedCourse) != 0) continue;
            for (i = 0; i < studentCount; i++) {
                if (strcmp(sid, students[i][0]) == 0) {
                    marks[i] = atoi(markStr);
                    break;
                }
            }
        }
        fclose(mf);
    }
    // --- Display students with marks ---
    printf("\n--- Student Marks for %s ---\n", selectedCourse);
    printf("------------------------------------------------\n");
    printf("%-15s %-25s %-10s\n", "Student ID", "Name", "Marks");
    printf("------------------------------------------------\n");
    for (i = 0; i < studentCount; i++) {
        if (marks[i] >= 0)
            printf("%-15s %-25s %-10d\n", students[i][0], students[i][1], marks[i]);
        else
            printf("%-15s %-25s %-10s\n", students[i][0], students[i][1], "N/A");
    }
}











//void create_quiz(const char *teacherID) {
//    char teacherCourses[4][20];
//    int numCourses = load_teacher_courses(teacherID, teacherCourses);
//
//    if (numCourses == 0) {
//        printf("No courses assigned.\n");
//        return;
//    }
//
//    printf("\n--- Create Quiz ---\n");
//    int i;
//    for (i = 0; i < numCourses; i++)
//        printf("%d. %s\n", i+1, teacherCourses[i]);
//
//    char choice[10];
//    read_line("Select course number", choice, sizeof(choice));
//    if (!is_number(choice)) {
//        printf("Invalid input.\n");
//        return;
//    }
//
//    int c = atoi(choice);
//    if (c < 1 || c > numCourses) {
//        printf("Invalid course selection.\n");
//        return;
//    }
//
//    char selectedCourse[20];
//    strcpy(selectedCourse, teacherCourses[c-1]);
//
//    char quizID[10];
//    generate_quiz_id(quizID);
//
//    char title[100], marksStr[10], timeStr[10];
//    read_line("Quiz Title", title, sizeof(title));
//    read_line("Total Marks", marksStr, sizeof(marksStr));
//    read_line("Time (minutes)", timeStr, sizeof(timeStr));
//
//    int totalMarks = atoi(marksStr);
//    int timeLimit  = atoi(timeStr);
//
//    FILE *qf = fopen("data/quizzes.csv", "a");
//    if (!qf) {
//        printf("Error opening quizzes file.\n");
//        return;
//    }
//
//    fprintf(qf, "%s,%s,%s,%s,%d,%d\n",
//            quizID, selectedCourse, teacherID, title, totalMarks, timeLimit);
//    fclose(qf);
//
//    printf("Quiz created successfully! ID: %s\n", quizID);
//
//    add_quiz_questions(quizID);
//}

//void create_quiz(const char *teacherID) {
//    char courses[MAX_TEACHER_COURSES][20];
//    int numCourses = load_teacher_courses(teacherID, courses);
//
//    if (numCourses == 0) {
//        printf("No courses assigned.\n");
//        return;
//    }
//
//    printf("\nSelect Course:\n");
//    int i;
//    for (i = 0; i < numCourses; i++)
//        printf("%d. %s\n", i + 1, courses[i]);
//
//    char choice[10];
//    read_line("Enter choice", choice, sizeof(choice));
//    if (!is_number(choice)) return;
//
//    int c = atoi(choice);
//    if (c < 1 || c > numCourses) {
//        printf("Invalid course selection.\n");
//        return;
//    }
//
//    char quizTitle[100];
//    read_line("Enter Quiz Title", quizTitle, sizeof(quizTitle));
//
//    int totalQuestions;
//    read_line("Number of Questions", choice, sizeof(choice));
//    totalQuestions = atoi(choice);
//
//    // generate quizID
//    char quizID[10];
//    generate_quiz_id(quizID);
//
//    save_quiz_header(quizID, teacherID, courses[c-1], quizTitle, totalQuestions);
//
//    for (i = 1; i <= totalQuestions; i++) {
//        add_quiz_question(quizID, i);
//    }
//
//    printf("Quiz created successfully!\n");
//}


//void add_quiz_questions(const char *quizID) {
//    FILE *f = fopen("data/quiz_questions.csv", "a");
//    if (!f) {
//        printf("Error opening quiz questions file.\n");
//        return;
//    }
//
//    char buf[256];
//    int qNo = 1;
//
//    while (1) {
//        printf("\n--- Question %d ---\n", qNo);
//
//        char question[200], A[100], B[100], C[100], D[100], correct[5];
//
//        read_line("Question", question, sizeof(question));
//        read_line("Option A", A, sizeof(A));
//        read_line("Option B", B, sizeof(B));
//        read_line("Option C", C, sizeof(C));
//        read_line("Option D", D, sizeof(D));
//        read_line("Correct Option (A/B/C/D)", correct, sizeof(correct));
//
//        fprintf(f, "%s,%d,%s,%s,%s,%s,%s,%s\n",
//                quizID, qNo, question, A, B, C, D, correct);
//
//        read_line("Add another question? (y/n)", buf, sizeof(buf));
//        if (buf[0] != 'y' && buf[0] != 'Y')
//            break;
//
//        qNo++;
//    }
//
//    fclose(f);
//    printf("Quiz questions saved successfully.\n");
//}


//helper functions:
//int load_teacher_courses(const char *teacherID, char courses[][20]) {
//    FILE *f = fopen("data/teacher_courses.csv", "r");
//    if (!f) {
//        printf("Error: teacher_courses.csv not found!\n");
//        return 0;
//    }
//
//    char line[512];
//    int count = 0;
//
//    while (fgets(line, sizeof(line), f)) {
//        char copy[512];
//        strcpy(copy, line);
//
//        char *token = strtok(copy, ",");
//        if (!token) continue;
//
//        trim(token);
//        if (strcmp(token, teacherID) != 0)
//            continue;
//
//        /* skip name & department */
//        strtok(NULL, ",");
//        strtok(NULL, ",");
//
//        /* read courses */
//        while ((token = strtok(NULL, ",\n")) != NULL && count < MAX_TEACHER_COURSES) {
//            trim(token);
//            strcpy(courses[count++], token);
//        }
//
//        break; // teacher found, stop reading
//    }
//
//    fclose(f);
//    return count;
//}
//
//void generate_quiz_id(char *quizID) {
//    static int counter = 1;
//    sprintf(quizID, "QZ%03d", counter++);
//}
