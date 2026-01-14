#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "../include/utils.h"
#include "../include/student.h"

#define STUDENT_COURSES_FILE "data/student_courses.csv"

void view_student_courses(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char sid[20];
    strncpy(sid, studentID, sizeof(sid)-1);
    sid[sizeof(sid)-1] = '\0';
    trim(sid);
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
        printf("Student courses file not found!\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tokens[50];
        int count = 0;
        char *tok = strtok(copy, ",");
        while (tok && count < 50) {
		    tok[strcspn(tok, "\n")] = '\0';
		    trim(tok);
		    if (strlen(tok) > 0) {   // <-- KEY FIX
		        tokens[count++] = tok;
		    }
		    tok = strtok(NULL, ",");
		}
        if (count < 3)
            continue;
        if (strcmp(tokens[0], sid) != 0)
            continue;
        found = 1;
        printf("\n--- My Enrolled Courses ---\n");
        printf("Student ID : %s\n", tokens[0]);
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
        printf("\nTotal Courses Enrolled: %d\n",
               count > 3 ? count - 3 : 0);
        break;
    }
    fclose(f);
    if (!found) {
        printf("No courses enrolled.\n");
    }
}

void view_student_marks(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/course_marks.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
        printf("Marks file not found!\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char sid[20];
    strncpy(sid, studentID, sizeof(sid) - 1);
    sid[sizeof(sid) - 1] = '\0';
    trim(sid);
    char line[256];
    int found = 0;
    printf("\n--- My Marks ---\n");
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *courseID = strtok(copy, ",");
        char *student  = strtok(NULL, ",");
        char *marks    = strtok(NULL, ",\n");
        if (!courseID || !student || !marks)
            continue;
        trim(courseID);
        trim(student);
        trim(marks);
        if (strcmp(student, sid) == 0) {
            found = 1;
            char courseName[100] = "Unknown Course";
            get_course_name(courseID, courseName, sizeof(courseName));
            printf("Course: %-30s Marks: %s\n", courseName, marks);
        }
    }
    fclose(f);

    if (!found) {
        printf("No marks available yet.\n");
    }
}

void attempt_quiz(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char quizIDs[20][20];
    char quizTitles[20][100];
    int num = load_available_quizzes(studentID, quizIDs, quizTitles, 20);
    if (num == 0) {
        printf("No quizzes available to attempt.\n");
        return;
    }
    printf("\n--- Available Quizzes ---\n");
    int i;
    for (i = 0; i < num; i++) {
        printf("%d. %s\n", i + 1, quizTitles[i]);
    }
    char buf[10];
    read_line("Select quiz number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid input.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    int choice = atoi(buf);
    if (choice < 1 || choice > num) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid quiz selection.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char quizID[20];
    strcpy(quizID, quizIDs[choice - 1]);
    // ?? Confirmation
    char confirm[5];
    int timeLimitMin = get_quiz_time_limit(quizID);
    printf("\nWARNING: Once you start the quiz, it cannot be re-attempted.\n");
    printf("Time Limit of this quiz: %d minutes\n",timeLimitMin);
    read_line("Start quiz? (Y/N)", confirm, sizeof(confirm));
    if (confirm[0] != 'Y' && confirm[0] != 'y') {
        printf("Quiz cancelled.\n");
        return;
    }
    // Start quiz
    int score = conduct_quiz(quizID);
    // Save attempt
    FILE *fa = fopen("data/quiz_attempts.csv", "a");
    fprintf(fa, "%s,%s\n", quizID, studentID);
    fclose(fa);
    // Save result
    FILE *fr = fopen("data/quiz_results.csv", "a");
    fprintf(fr, "%s,%s,%d\n", quizID, studentID, score);
    fclose(fr);
    printf("\nQuiz completed! Your score: %d\n", score);
}

//int conduct_quiz(const char *quizID) {
//    FILE *f = fopen("data/quiz_questions.csv", "r");
//    if (!f) return 0;
//    char line[512];
//    int score = 0;
//    while (fgets(line, sizeof(line), f)) {
//        char copy[512];
//        strcpy(copy, line);
//        char *qid = strtok(copy, ",");
//        char *qno = strtok(NULL, ",");
//        char *q   = strtok(NULL, ",");
//        char *A   = strtok(NULL, ",");
//        char *B   = strtok(NULL, ",");
//        char *C   = strtok(NULL, ",");
//        char *D   = strtok(NULL, ",");
//        char *correct = strtok(NULL, ",\n");
//        if (!qid || strcmp(qid, quizID) != 0)
//            continue;
//        printf("\nQ%s: %s\n", qno, q);
//        printf("A) %s\nB) %s\nC) %s\nD) %s\n", A, B, C, D);
//        char ans[5];
//        read_line("Your answer", ans, sizeof(ans));
//        if (toupper(ans[0]) == toupper(correct[0])) {
//            score++;
//        }
//    }
//    fclose(f);
//    return score;
//}

int conduct_quiz(const char *quizID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int timeLimitMin = get_quiz_time_limit(quizID);
    if (timeLimitMin <= 0) {
        printf("Quiz time limit not found.\n");
        return 0;
    }
    time_t startTime = time(NULL);
    int timeLimitSec = timeLimitMin * 60;
    FILE *f = fopen("data/quiz_questions.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
        printf("Quiz questions file not found.\n");
    	SetConsoleTextAttribute(h, 10);
        return 0;
    }
    char line[512];
    int score = 0;
    printf("\nQuiz started! Time limit: %d minutes\n", timeLimitMin);
    printf("--------------------------------------------------\n");
    while (fgets(line, sizeof(line), f)) {
        /* ? TIME CHECK */
        time_t now = time(NULL);
        if (difftime(now, startTime) >= timeLimitSec) {
            printf("\nTime is up! Quiz auto-submitted.\n");
            break;
        }
        char copy[512];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        char *qno = strtok(NULL, ",");
        char *q   = strtok(NULL, ",");
        char *A   = strtok(NULL, ",");
        char *B   = strtok(NULL, ",");
        char *C   = strtok(NULL, ",");
        char *D   = strtok(NULL, ",");
        char *correct = strtok(NULL, ",\n");
        if (!qid || strcmp(qid, quizID) != 0)
            continue;
        printf("\nQ%s: %s\n", qno, q);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n", A, B, C, D);
        int remaining = timeLimitSec - (int)difftime(time(NULL), startTime);
        printf("Remaining time: %d sec\n", remaining);
        char ans[5];
        read_line("Your answer", ans, sizeof(ans));
        if (strlen(ans) == 0) {
            printf("No answer entered. Moving on.\n");
            continue;
        }
        if (toupper(ans[0]) == toupper(correct[0])) {
            score++;
        }
    }
    fclose(f);
    printf("\nQuiz finished!\n");
    printf("Your Score: %d\n", score);
    return score;
}

void view_quiz_result(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *fr = fopen("data/quiz_results.csv", "r");
    if (!fr) {
        printf("No quiz results available.\n");
        return;
    }
    char line[256];
    char quizIDs[20][20];
    int scores[20];
    int count = 0;
    while (fgets(line, sizeof(line), fr)) {
        char *qid = strtok(line, ",");
        char *sid = strtok(NULL, ",");
        char *score = strtok(NULL, ",\n");
        if (!qid || !sid || !score) continue;
        trim(qid);
        trim(sid);
        if (strcmp(sid, studentID) == 0) {
            strcpy(quizIDs[count], qid);
            scores[count] = atoi(score);
            count++;
        }
    }
    fclose(fr);
    if (count == 0) {
        printf("No quiz results found.\n");
        return;
    }
    printf("\n--- My Quiz Results ---\n");
    int i;
    for (i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, quizIDs[i]);
    }
    char buf[10];
    read_line("Select quiz number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid input.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    int choice = atoi(buf);
    if (choice < 1 || choice > count) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid selection.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedQuiz[20];
    strcpy(selectedQuiz, quizIDs[choice - 1]);
    int score = scores[choice - 1];
    /* Fetch quiz info */
    FILE *fq = fopen("data/quizzes.csv", "r");
    if (!fq) {
        printf("Quiz file missing.\n");
        return;
    }
    char title[100] = "Unknown";
    char course[20] = "Unknown";
    int totalQ = 0;
    while (fgets(line, sizeof(line), fq)) {
        char *qid = strtok(line, ",");
        strtok(NULL, ",");              // teacherID
        char *cid = strtok(NULL, ",");
        char *qt  = strtok(NULL, ",");
        char *tq  = strtok(NULL, ",\n");
        if (qid && strcmp(qid, selectedQuiz) == 0) {
            trim(cid);
            trim(qt);
            trim(tq);
            strcpy(course, cid);
            strcpy(title, qt);
            totalQ = atoi(tq);
            break;
        }
    }
    fclose(fq);
    float percent = (totalQ > 0)
        ? ((float)score / totalQ) * 100
        : 0;
    printf("\n--- Quiz Result ---\n");
    printf("Quiz ID   : %s\n", selectedQuiz);
    printf("Title     : %s\n", title);
    printf("Course    : %s\n", course);
    printf("Score     : %d / %d\n", score, totalQ);
    printf("Percentage: %.2f%%\n", percent);
}

void student_change_password(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/students.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
        printf("Students file not found.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    FILE *temp = fopen("data/students_temp.csv", "w");
    if (!temp) {
        fclose(f);
        SetConsoleTextAttribute(h, 12);
        printf("Error creating temp file.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char current[50], newpass[50], confirm[50];
    read_line("Enter current password", current, sizeof(current));
    int found = 0;
    int updated = 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *id   = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *pass = strtok(NULL, ",\n");
        if (!id || !name || !dept || !pass) {
            fputs(line, temp);
            continue;
        }
        trim(id); trim(pass);
        if (strcmp(id, studentID) == 0) {
            found = 1;
            if (strcmp(pass, current) != 0) {
            	SetConsoleTextAttribute(h, 12);
		        printf("Incorrect current password.\n");
		    	SetConsoleTextAttribute(h, 10);
                fputs(line, temp);
                continue;
            }
            read_line("Enter new password", newpass, sizeof(newpass));
            if (strlen(newpass) == 0) {
            	SetConsoleTextAttribute(h, 12);
		        printf("New password cannot be empty.\n");
		    	SetConsoleTextAttribute(h, 10);
                fputs(line, temp);
                continue;
            }
            read_line("Confirm new password", confirm, sizeof(confirm));
            if (strcmp(newpass, confirm) != 0) {
            	SetConsoleTextAttribute(h, 12);
		        printf("Passwords do not match.\n");
		    	SetConsoleTextAttribute(h, 10);
                fputs(line, temp);
                continue;
            }
            fprintf(temp, "%s,%s,%s,%s\n",
                    id, name, dept, newpass);
            updated = 1;
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    if (!found) {
        remove("data/students_temp.csv");
        SetConsoleTextAttribute(h, 12);
        printf("Student record not found.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    if (!updated) {
        remove("data/students_temp.csv");
        return;
    }
    remove("data/students.csv");
    rename("data/students_temp.csv", "data/students.csv");
    printf("Password changed successfully!\n");
}

void view_student_gpa(const char *studentID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/student_gpa.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
        printf("\n[GPA Error] student_gpa.csv not found.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[128];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *id = strtok(line, ",");
        char *gpaStr = strtok(NULL, "\n");
        if (!id || !gpaStr)
            continue;
        if (strcmp(id, studentID) == 0) {
            printf("\nYour GPA: %s\n", gpaStr);
            found = 1;
            break;
        }
    }
    fclose(f);
    if (!found) {
        printf("\nNo GPA record found for your ID yet.\n");
    }
}


//Helper functions:
int get_course_name(const char *courseID, char *courseName, int size) {
    FILE *f = fopen("data/courses.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id   = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        if (!id || !name) continue;
        trim(id);
        trim(name);
        if (strcmp(id, courseID) == 0) {
            strncpy(courseName, name, size - 1);
            courseName[size - 1] = '\0';
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int has_attempted_quiz(const char *quizID, const char *studentID) {
    FILE *f = fopen("data/quiz_attempts.csv", "r");
    if (!f) return 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char qid[50], sid[50];
        sscanf(line, "%[^,],%s", qid, sid);
        trim(qid);
        trim(sid);
        if (strcmp(qid, quizID) == 0 && strcmp(sid, studentID) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int load_available_quizzes(const char *studentID,char quizIDs[][20],char quizTitles[][100],int max) {
    FILE *f = fopen("data/quizzes.csv", "r");
    if (!f) return 0;
    char line[512];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *qid   = strtok(copy, ",");
        strtok(NULL, ","); // teacher
        char *course = strtok(NULL, ",");
        char *title  = strtok(NULL, ",");
        if (!qid || !course || !title)
            continue;
        trim(qid);
        trim(course);
        trim(title);
        // student must be enrolled in this course
        if (!is_student_enrolled(studentID, course))
            continue;
        // must not be attempted
        if (has_attempted_quiz(qid, studentID))
            continue;
        strcpy(quizIDs[count], qid);
        strcpy(quizTitles[count], title);
        count++;
        if (count == max)
            break;
    }
    fclose(f);
    return count;
}

int is_student_enrolled(const char *studentID, const char *courseID) {
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) return 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *tokens[50];
        int count = 0;
        char *tok = strtok(copy, ",");
        while (tok && count < 50) {
            tok[strcspn(tok, "\n")] = '\0';
            trim(tok);
            tokens[count++] = tok;
            tok = strtok(NULL, ",");
        }
        if (count < 4)
            continue;
        if (strcmp(tokens[0], studentID) != 0)
            continue;
        // courses start from index 3
        int i;
        for (i = 3; i < count; i++) {
            if (strcmp(tokens[i], courseID) == 0) {
                fclose(f);
                return 1;  // enrolled
            }
        }
        fclose(f);
        return 0; // student found but course not enrolled
    }
    fclose(f);
    return 0; // student not found
}

int get_quiz_time_limit(const char *quizID) {
    FILE *f = fopen("data/quizzes.csv", "r");
    if (!f) return 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        strtok(NULL, ","); // teacherID
        strtok(NULL, ","); // courseID
        strtok(NULL, ","); // title
        strtok(NULL, ","); // total questions
        char *time = strtok(NULL, ",\n");
        if (qid && time && strcmp(qid, quizID) == 0) {
            fclose(f);
            return atoi(time); // minutes
        }
    }
    fclose(f);
    return 0;
}

