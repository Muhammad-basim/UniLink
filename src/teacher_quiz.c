#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/utils.h"
#include "../include/teacher_quiz.h"

//void create_quiz(const char *teacherID) {
//    char teacherCourses[4][20];
//    int numCourses = load_teacher_courses(teacherID, teacherCourses);
//    if (numCourses == 0) {
//        printf("You have no assigned courses.\n");
//        return;
//    }
//    printf("\n--- Select Course ---\n");
//    int i;
//    for (i = 0; i < numCourses; i++) {
//        printf("%d. %s\n", i + 1, teacherCourses[i]);
//    }
//    char buf[10];
//    read_line("Enter course number", buf, sizeof(buf));
//    if (!is_number(buf)) {
//        printf("Invalid input.\n");
//        return;
//    }
//    int choice = atoi(buf);
//    if (choice < 1 || choice > numCourses) {
//        printf("Invalid course selection.\n");
//        return;
//    }
//    char courseID[20];
//    strcpy(courseID, teacherCourses[choice - 1]);
//    char quizID[20], title[100];
//    read_line("Quiz ID", quizID, sizeof(quizID));
//    if (strlen(quizID) == 0) {
//        printf("Quiz ID cannot be empty.\n");
//        return;
//    }
//    if (quiz_id_exists(quizID)) {
//	    printf("Quiz ID '%s' already exists. Please choose a unique ID.\n", quizID);
//	    return;
//	}
//    read_line("Quiz Title", title, sizeof(title));
//    if (strlen(title) == 0) {
//        printf("Quiz title cannot be empty.\n");
//        return;
//    }
//    read_line("Total Questions", buf, sizeof(buf));
//    if (!is_number(buf) || atoi(buf) <= 0) {
//        printf("Invalid number of questions.\n");
//        return;
//    }
//    int totalQ = atoi(buf);
//    FILE *fq = fopen("data/quizzes.csv", "a");
//    FILE *qq = fopen("data/quiz_questions.csv", "a");
//    if (!fq || !qq) {
//        printf("File error.\n");
//        if (fq) fclose(fq);
//        if (qq) fclose(qq);
//        return;
//    }
//    /* Save quiz header ONCE */
//    fprintf(fq, "%s,%s,%s,%s,%d\n",
//            quizID, teacherID, courseID, title, totalQ);
//    /* Add questions */
//    for (i = 1; i <= totalQ; i++) {
//        char question[200], A[100], B[100], C[100], D[100], correct[5];
//        printf("\nQuestion %d\n", i);
//        read_line("Question", question, sizeof(question));
//        if (!strlen(question)) { i--; continue; }
//        read_line("Option A", A, sizeof(A));
//        read_line("Option B", B, sizeof(B));
//        read_line("Option C", C, sizeof(C));
//        read_line("Option D", D, sizeof(D));
//        if (!strlen(A) || !strlen(B) || !strlen(C) || !strlen(D)) {
//            printf("Options cannot be empty.\n");
//            i--; continue;
//        }
//        read_line("Correct Option (A/B/C/D)", correct, sizeof(correct));
//        if (strlen(correct) != 1 || !strchr("ABCD", correct[0])) {
//            printf("Invalid correct option.\n");
//            i--; continue;
//        }
//        fprintf(qq, "%s,%d,%s,%s,%s,%s,%s,%s\n",
//                quizID, i, question, A, B, C, D, correct);
//    }
//    fclose(fq);
//    fclose(qq);
//    printf("\nQuiz created successfully!\n");
//}

void create_quiz(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char teacherCourses[4][20];
    int numCourses = load_teacher_courses(teacherID, teacherCourses);
    if (numCourses == 0) {
        printf("You have no assigned courses.\n");
        return;
    }
    printf("\n--- Select Course ---\n");
    int i;
    for (i = 0; i < numCourses; i++) {
        printf("%d. %s\n", i + 1, teacherCourses[i]);
    }
    char buf[10];
    read_line("Enter course number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid input.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    int choice = atoi(buf);
    if (choice < 1 || choice > numCourses) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid course selection.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    char courseID[20];
    strcpy(courseID, teacherCourses[choice - 1]);
    char quizID[20], title[100];
    read_line("Quiz ID", quizID, sizeof(quizID));
    if (!strlen(quizID)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Quiz ID cannot be empty.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    if (quiz_id_exists(quizID)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Quiz ID '%s' already exists. Please choose a unique ID.\n", quizID);
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    read_line("Quiz Title", title, sizeof(title));
    if (!strlen(title)) {
    	SetConsoleTextAttribute(h, 12);
        printf("Quiz title cannot be empty.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    read_line("Total Questions", buf, sizeof(buf));
    if (!is_number(buf) || atoi(buf) <= 0) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid number of questions.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    int totalQ = atoi(buf);
    /* NEW: Time limit */
    read_line("Time Limit (minutes)", buf, sizeof(buf));
    if (!is_number(buf) || atoi(buf) <= 0) {
    	SetConsoleTextAttribute(h, 12);
        printf("Invalid time limit.\n");
    	SetConsoleTextAttribute(h, 10);
        return;
    }
    int timeLimit = atoi(buf);
    FILE *fq = fopen("data/quizzes.csv", "a");
    FILE *qq = fopen("data/quiz_questions.csv", "a");
    if (!fq || !qq) {
        printf("File error.\n");
        if (fq) fclose(fq);
        if (qq) fclose(qq);
        return;
    }
    /* Save quiz header ONCE */
    fprintf(fq, "%s,%s,%s,%s,%d,%d\n",
            quizID, teacherID, courseID, title, totalQ, timeLimit);
    /* Add questions */
    for (i = 1; i <= totalQ; i++) {
        char question[200], A[100], B[100], C[100], D[100], correct[5];
        printf("\nQuestion %d\n", i);
        read_line("Question", question, sizeof(question));
        if (!strlen(question)) { i--; continue; }
        read_line("Option A", A, sizeof(A));
        read_line("Option B", B, sizeof(B));
        read_line("Option C", C, sizeof(C));
        read_line("Option D", D, sizeof(D));
        if (!strlen(A) || !strlen(B) || !strlen(C) || !strlen(D)) {
        	SetConsoleTextAttribute(h, 12);
		    printf("Options cannot be empty.\n");
			SetConsoleTextAttribute(h, 10);
            i--; continue;
        }
        read_line("Correct Option (A/B/C/D)", correct, sizeof(correct));
        if (strlen(correct) != 1 || !strchr("ABCD", correct[0])) {
        	SetConsoleTextAttribute(h, 12);
		    printf("Invalid correct option.\n");
			SetConsoleTextAttribute(h, 10);
            i--; continue;
        }
        fprintf(qq, "%s,%d,%s,%s,%s,%s,%s,%s\n",
                quizID, i, question, A, B, C, D, correct);
    }
    fclose(fq);
    fclose(qq);
    printf("\nQuiz created successfully with time limit of %d minutes!\n", timeLimit);
}


void view_my_quizzes(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char teacherCourses[4][20];
    int numCourses = load_teacher_courses(teacherID, teacherCourses);
    if (numCourses == 0) {
        printf("You have no assigned courses.\n");
        return;
    }
    /* --- Select Course --- */
    printf("\n--- Select Course ---\n");
    int i;
    for (i = 0; i < numCourses; i++) {
        printf("%d. %s\n", i + 1, teacherCourses[i]);
    }
    char buf[10];
    read_line("Enter course number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    int c = atoi(buf);
    if (c < 1 || c > numCourses) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid course selection.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedCourse[20];
    strcpy(selectedCourse, teacherCourses[c - 1]);
    /* --- Load quizzes for this course --- */
    FILE *fq = fopen("data/quizzes.csv", "r");
    if (!fq) {
    	SetConsoleTextAttribute(h, 12);
		printf("No quizzes found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[512];
    char quizIDs[20][20];
    char quizTitles[20][100];
    int quizCount = 0;
    while (fgets(line, sizeof(line), fq)) {
        char copy[512];
        strcpy(copy, line);
        char *quizID = strtok(copy, ",");
        char *tid    = strtok(NULL, ",");
        char *course = strtok(NULL, ",");
        char *title  = strtok(NULL, ",");
        strtok(NULL, ","); // total questions
        if (!quizID || !tid || !course || !title) continue;
        if (strcmp(tid, teacherID) == 0 &&
            strcmp(course, selectedCourse) == 0) {
            strcpy(quizIDs[quizCount], quizID);
            strcpy(quizTitles[quizCount], title);
            quizCount++;
        }
    }
    fclose(fq);
    if (quizCount == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No quizzes found for this course.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    /* --- Select Quiz --- */
    printf("\n--- Select Quiz ---\n");
    for (i = 0; i < quizCount; i++) {
        printf("%d. %s (%s)\n", i + 1, quizIDs[i], quizTitles[i]);
    }
    read_line("Enter quiz number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    int q = atoi(buf);
    if (q < 1 || q > quizCount) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid quiz selection.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedQuizID[20];
    strcpy(selectedQuizID, quizIDs[q - 1]);
    /* --- Display Quiz Header --- */
    fq = fopen("data/quizzes.csv", "r");
    while (fgets(line, sizeof(line), fq)) {
        char copy[512];
        strcpy(copy, line);
        char *quizID = strtok(copy, ",");
        char *tid    = strtok(NULL, ",");
        char *course = strtok(NULL, ",");
        char *title  = strtok(NULL, ",");
        char *tq     = strtok(NULL, ",");
        if (quizID && strcmp(quizID, selectedQuizID) == 0) {
            printf("\n--- Quiz Details ---\n");
            printf("Quiz ID : %s\n", quizID);
            printf("Course  : %s\n", course);
            printf("Title   : %s\n", title);
            printf("Total Questions: %s\n\n", tq);
            break;
        }
    }
    fclose(fq);
    /* --- Display Questions --- */
    FILE *qq = fopen("data/quiz_questions.csv", "r");
    if (!qq) {
    	SetConsoleTextAttribute(h, 12);
		printf("Questions file not found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    printf("--- Questions ---\n");
    while (fgets(line, sizeof(line), qq)) {
        char copy[512];
        strcpy(copy, line);
        char *quizID = strtok(copy, ",");
        char *qno    = strtok(NULL, ",");
        char *ques   = strtok(NULL, ",");
        char *A      = strtok(NULL, ",");
        char *B      = strtok(NULL, ",");
        char *C      = strtok(NULL, ",");
        char *D      = strtok(NULL, ",");
        char *corr   = strtok(NULL, ",\n");
        if (quizID && strcmp(quizID, selectedQuizID) == 0) {
            printf("\nQ%s. %s\n", qno, ques);
            printf(" A) %s\n", A);
            printf(" B) %s\n", B);
            printf(" C) %s\n", C);
            printf(" D) %s\n", D);
            printf(" Correct Answer: %s\n", corr);
        }
    }
    fclose(qq);
}

void delete_quiz(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char teacherCourses[4][20];
    int numCourses = load_teacher_courses(teacherID, teacherCourses);
    if (numCourses == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("You have no assigned courses.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    /* --- Select Course --- */
    printf("\n--- Select Course ---\n");
    int i;
    for (i = 0; i < numCourses; i++) {
        printf("%d. %s\n", i + 1, teacherCourses[i]);
    }
    char buf[10];
    read_line("Enter course number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    int c = atoi(buf);
    if (c < 1 || c > numCourses) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid course selection.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedCourse[20];
    strcpy(selectedCourse, teacherCourses[c - 1]);
    /* --- Load quizzes for course --- */
    FILE *fq = fopen("data/quizzes.csv", "r");
    if (!fq) {
    	SetConsoleTextAttribute(h, 12);
		printf("No quizzes found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char quizIDs[20][20];
    char quizTitles[20][100];
    int quizCount = 0;
    char line[512];
    while (fgets(line, sizeof(line), fq)) {
        char copy[512];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        char *tid = strtok(NULL, ",");
        char *cid = strtok(NULL, ",");
        char *title = strtok(NULL, ",");
        if (!qid || !tid || !cid || !title) continue;
        if (strcmp(tid, teacherID) == 0 &&
            strcmp(cid, selectedCourse) == 0) {
            strcpy(quizIDs[quizCount], qid);
            strcpy(quizTitles[quizCount], title);
            quizCount++;
        }
    }
    fclose(fq);
    if (quizCount == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No quizzes found for this course.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    /* --- Select Quiz --- */
    printf("\n--- Select Quiz to Delete ---\n");
    for (i = 0; i < quizCount; i++) {
        printf("%d. %s (%s)\n", i + 1, quizIDs[i], quizTitles[i]);
    }
    read_line("Enter quiz number", buf, sizeof(buf));
    if (!is_number(buf)) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid input.\n");
		SetConsoleTextAttribute(h, 10);
        return;
	}
    int q = atoi(buf);
    if (q < 1 || q > quizCount) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid quiz selection.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedQuizID[20];
    strcpy(selectedQuizID, quizIDs[q - 1]);
    /* --- Confirm Delete --- */
    read_line("Are you sure? (Y/N)", buf, sizeof(buf));
    if (buf[0] != 'Y' && buf[0] != 'y') {
        printf("Delete cancelled.\n");
        return;
    }
    /* --- Delete from quizzes.csv --- */
    FILE *in = fopen("data/quizzes.csv", "r");
    FILE *out = fopen("data/temp.csv", "w");
    while (fgets(line, sizeof(line), in)) {
        char copy[512];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        if (qid && strcmp(qid, selectedQuizID) == 0)
            continue;  // skip quiz
        fputs(line, out);
    }
    fclose(in);
    fclose(out);
    remove("data/quizzes.csv");
    rename("data/temp.csv", "data/quizzes.csv");
    /* --- Delete from quiz_questions.csv --- */
    in = fopen("data/quiz_questions.csv", "r");
    out = fopen("data/temp.csv", "w");
    while (fgets(line, sizeof(line), in)) {
        char copy[512];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        if (qid && strcmp(qid, selectedQuizID) == 0)
            continue;  // skip questions
        fputs(line, out);
    }
    fclose(in);
    fclose(out);
    remove("data/quiz_questions.csv");
    rename("data/temp.csv", "data/quiz_questions.csv");
    printf("\nQuiz '%s' deleted successfully.\n", selectedQuizID);
}

void update_quiz(const char *teacherID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    char quizIDs[20][20], titles[20][100];
    int quizCount = load_teacher_quizzes(teacherID, quizIDs, titles);
    if (quizCount == 0) {
        printf("You have no quizzes.\n");
        return;
    }
    printf("\n--- Select Quiz to Update ---\n");
    int i;
    for (i = 0; i < quizCount; i++) {
        printf("%d. %s (%s)\n", i + 1, quizIDs[i], titles[i]);
    }
    char buf[10];
    read_line("Enter quiz number", buf, sizeof(buf));
    if (!is_number(buf)) return;
    int choice = atoi(buf);
    if (choice < 1 || choice > quizCount) {
    	SetConsoleTextAttribute(h, 12);
		printf("Invalid selection.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char selectedQuiz[20];
    strcpy(selectedQuiz, quizIDs[choice - 1]);
    edit_quiz_questions(selectedQuiz);
}

void edit_quiz_questions(const char *quizID) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/quiz_questions.csv", "r");
    if (!f) {
    	SetConsoleTextAttribute(h, 12);
		printf("Questions file not found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char line[1024];
    int questionCount = 0;
    printf("\n--- Questions for Quiz %s ---\n", quizID);
    while (fgets(line, sizeof(line), f)) {
        char copy[1024];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        char *qno = strtok(NULL, ",");
        char *question = strtok(NULL, ",");
        if (qid && strcmp(qid, quizID) == 0) {
            printf("%s. %s\n", qno, question);
            questionCount++;
        }
    }
    fclose(f);
    if (questionCount == 0) {
    	SetConsoleTextAttribute(h, 12);
		printf("No questions found.\n");
		SetConsoleTextAttribute(h, 10);
        return;
    }
    char buf[10];
    read_line("Enter question number to edit", buf, sizeof(buf));
    if (!is_number(buf)) return;
    int qnum = atoi(buf);
    update_single_question(quizID, qnum);
}

void update_single_question(const char *quizID, int qno) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *f = fopen("data/quiz_questions.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!f || !temp) return;
    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[1024];
        strcpy(copy, line);
        char *qid = strtok(copy, ",");
        char *num = strtok(NULL, ",");
        char *oldQ = strtok(NULL, ",");
        char *oldA = strtok(NULL, ",");
        char *oldB = strtok(NULL, ",");
        char *oldC = strtok(NULL, ",");
        char *oldD = strtok(NULL, ",");
        char *oldCorrect = strtok(NULL, ",\n");
        if (qid && num &&
            strcmp(qid, quizID) == 0 &&
            atoi(num) == qno) {
            found = 1;
            char question[200], A[100], B[100], C[100], D[100], correct[5];
            printf("\n(Press Enter to keep old value)\n");
            printf("Old Question: %s\n", oldQ);
            read_line("New Question", question, sizeof(question));
            keep_or_update(question, oldQ);
            printf("Old Option A: %s\n", oldA);
            read_line("Option A", A, sizeof(A));
            keep_or_update(A, oldA);
            printf("Old Option B: %s\n", oldB);
            read_line("Option B", B, sizeof(B));
            keep_or_update(B, oldB);
            printf("Old Option C: %s\n", oldC);
            read_line("Option C", C, sizeof(C));
            keep_or_update(C, oldC);
            printf("Old Option D: %s\n", oldD);
            read_line("Option D", D, sizeof(D));
            keep_or_update(D, oldD);
            printf("Old Correct Option: %s\n", oldCorrect);
            read_line("Correct Option (A/B/C/D)", correct, sizeof(correct));
            keep_or_update(correct, oldCorrect);
            fprintf(temp, "%s,%d,%s,%s,%s,%s,%s,%s\n",
                    quizID, qno,
                    question, A, B, C, D, correct);
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    remove("data/quiz_questions.csv");
    rename("data/temp.csv", "data/quiz_questions.csv");
    if (found)
        printf("Question updated successfully!\n");
    else{
    	SetConsoleTextAttribute(h, 12);
		printf("Question not found.\n");
		SetConsoleTextAttribute(h, 10);
	}
}



//Helper functions: 
int load_teacher_courses(const char *teacherID, char courses[][20]) {
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) return 0;
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (!tid || strcmp(tid, teacherID) != 0)
            continue;
        strtok(NULL, ","); // teacher name
        strtok(NULL, ","); // department
        char *course = strtok(NULL, ",\n");
        while (course && count < 4) {
            strcpy(courses[count++], course);
            course = strtok(NULL, ",\n");
        }
        break; // teacher found, no need to read further
    }
    fclose(f);
    return count;
}

int quiz_id_exists(const char *quizID) {
    FILE *f = fopen("data/quizzes.csv", "r");
    if (!f) return 0;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);

        char *qid = strtok(copy, ",");
        if (qid && strcmp(qid, quizID) == 0) {
            fclose(f);
            return 1;  // already exists
        }
    }
    fclose(f);
    return 0;
}

int load_teacher_quizzes(const char *teacherID, char quizIDs[][20], char titles[][100]) {
    FILE *f = fopen("data/quizzes.csv", "r");
    if (!f) return 0;

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);

        char *qid = strtok(copy, ",");
        char *tid = strtok(NULL, ",");
        strtok(NULL, ",");   // course
        char *title = strtok(NULL, ",");

        if (qid && tid && strcmp(tid, teacherID) == 0) {
            strcpy(quizIDs[count], qid);
            strcpy(titles[count], title);
            count++;
        }
    }
    fclose(f);
    return count;
}

void keep_or_update(char *newVal, const char *oldVal) {
    if (strlen(newVal) == 0) {
        strcpy(newVal, oldVal);
    }
}

