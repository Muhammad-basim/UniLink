#ifndef FILEIO_H
#define FILEIO_H

//Student Managment
int add_student(const char *id, const char *name, const char *dept, const char *pass);
void list_students(void);
int delete_student(const char *id);
int update_student(const char *id, const char *newName, const char *newDept, const char *newPass);
void search_student(const char *id);

//Teacher Managment
int add_teacher(const char *id, const char *name, const char *dept, const char *pass);
void list_teachers(void);
int delete_teacher(const char *id);
int update_teacher(const char *id, const char *newName, const char *newDept, const char *newPass);
void search_teacher(const char *id);

//Course Management
int add_course(const char *courseID, const char *name, int credits);
void list_courses(void);
void update_courses(const char *courseID, const char *newName, int newCredits);
int delete_course(const char *courseID);

int course_exists(const char *courseID);

int assign_course_to_student(const char *id, const char *courseID);
void list_student_courses(const char *id);
int remove_course_from_student(const char *id, const char *courseID);

int get_teacher_info(const char *id, char *name, char *dept);

int assign_course_to_teacher(const char *teacherID, const char *courseID);
void list_teacher_courses(const char *teacherID);
int remove_course_from_teacher(const char *teacherID, const char *courseID);

#endif

