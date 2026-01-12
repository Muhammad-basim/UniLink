#ifndef TEACHER_QUIZ_H
#define TEACHER_QUIZ_H

void manage_quiz_menu(const char *teacherID);

/* core features */
void create_quiz(const char *teacherID);
void view_my_quizzes(const char *teacherID);
void delete_quiz(const char *teacherID);
void update_quiz(const char *teacherID);

#endif

