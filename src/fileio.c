#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // for isspace()
#include "../include/fileio.h"
#include "../include/utils.h"

#define STUDENTS_FILE "data/students.csv"
#define TEACHERS_FILE "data/teachers.csv"
#define COURSES_FILE "data/courses.csv"

#define LINE_LEN 256

//Student Management
int add_student(const char *id, const char *name, const char *dept, const char *pass) {
    FILE *f = fopen(STUDENTS_FILE, "a");
    if (!f) {
        printf("Failed to open students file!\n");
        return 0;
    }
    fprintf(f, "%s,%s,%s,%s\n", id, name, dept, pass);
    fclose(f);
    printf("Student added successfully!\n");
    return 1;
}

void list_students(void) {
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
        printf("No students found!\n");
        return;
    }
    char line[LINE_LEN];
    printf("\n--- Students List ---\n");
    while (fgets(line, sizeof(line), f)) {
//        char *token = strtok(line, ",");
//        if (!token) continue;
//		char *roll = token ? token : "";
//		
//        token = strtok(NULL, ",");
//        char *name = token ? token : "";
//
//        token = strtok(NULL, ",");
//        char *dept = token ? token : "";
//
//        token = strtok(NULL, ",");
//        char *pass = token ? token : "";
		line[strcspn(line, "\r\n")] = 0;
		char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        if (!id || !name || !dept) continue;
        printf("ID: %-5s | Name: %-15s | Dept: %-10s\n", id, name, dept);
    }
    fclose(f);
}

int delete_student(const char *roll) {
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
        printf("No students found!\n");
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
    else
        printf("Student not found!\n");

    return found;
}


int update_student(const char *id, const char *newName, const char *newDept, const char *newPass) {
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
        printf("No students found!\n");
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
    else
        printf("Student not found!\n");

    return found;
}

void search_student(const char *id) {
    FILE *f = fopen(STUDENTS_FILE, "r");
    if (!f) {
        printf("No students found!\n");
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
            printf("ID: %s\n", file_roll);
            printf("Name: %s\n", name);
            printf("Department: %s\n", dept);
            printf("Password: %s\n", pass);
            break;
        }
    }

    if (!found) {
        printf("Student with ID %s not found!\n", id);
    }

    fclose(f);
}


//Teacher Management
int add_teacher(const char *id, const char *name, const char *dept, const char *pass) {
    FILE *f = fopen(TEACHERS_FILE, "a");
    if (!f) {
        printf("Failed to open teachers file!\n");
        return 0;
    }

    fprintf(f, "%s,%s,%s,%s\n", id, name, dept, pass);
    fclose(f);

    printf("Teacher added successfully!\n");
    return 1;
}

void list_teachers(void) {
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
        printf("No teachers found!\n");
        return;
    }

    char line[256];
    printf("\n--- Teachers List ---\n");

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

        printf("ID: %-5s | Name: %-15s | Dept: %-10s\n", id, name, dept);
    }

    fclose(f);
}

int delete_teacher(const char *id) {
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
        printf("No teachers found!\n");
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
    else
        printf("Teacher not found!\n");

    return found;
}

int update_teacher(const char *id, const char *newName, const char *newDept, const char *newPass) {
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
        printf("No teachers found!\n");
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
    else
        printf("Teacher not found!\n");

    return found;
}

void search_teacher(const char *id) {
    FILE *f = fopen(TEACHERS_FILE, "r");
    if (!f) {
        printf("No teachers found!\n");
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

    if (!found) printf("Teacher with ID %s not found!\n", id);

    fclose(f);
}

//Course Management
int add_course(const char *courseID, const char *name, int credits) {
    FILE *f = fopen(COURSES_FILE, "a");
    if (!f) {
        printf("Failed to open courses file!\n");
        return 0;
    }
    fprintf(f, "%s,%s,%d\n", courseID, name, credits);
    fclose(f);
    printf("Course with course ID %s added successfully!\n", courseID);
    return 1;
}

void list_courses(void) {
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
        printf("No courses found!\n");
        return;
    }
    char line[256];
    printf("\n--- Courses List ---\n");
    printf("%-10s | %-30s | Credits\n", "CourseID", "Course Name");
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
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
        printf("No Courses found!\n");
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
    else
        printf("Course with course ID %s not found!\n", courseID);
    return found;
}

int delete_course(const char *courseID) {
    FILE *f = fopen(COURSES_FILE, "r");
    if (!f) {
        printf("No courses found!\n");
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
    else
        printf("Course with course ID %s not found!\n", courseID);
    return found;
}

int assign_course_to_student(const char *studentID, const char *courseID) {
    FILE *sf = fopen("data/students.csv", "r");
    if (!sf) {
        printf("Students file not found!\n");
        return 0;
    }
    char student_name[50] = "";
    char student_dept[50] = "";
    char line[256];
    // ----- FIND STUDENT INFO -----
    while (fgets(line, sizeof(line), sf)) {
        char copy[256];
        strcpy(copy, line);
        char *id   = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        if (!id) continue;
        if (strcmp(id, studentID) == 0) {
            if (name) strncpy(student_name, name, sizeof(student_name)-1);
            if (dept) strncpy(student_dept, dept, sizeof(student_dept)-1);
            break;
        }
    }
    fclose(sf);
    if (strlen(student_name) == 0) {
        printf("Student with ID %s not found!\n", studentID);
        return 0;
    }
    if (!course_exists(courseID)) {
    	printf("Course with ID %s does not exist!\n", courseID);
    	return 0;
	}
    // ----- UPDATE student_courses.csv -----
    FILE *f = fopen("data/student_courses.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        if (f) fclose(f);
        return 0;
    }
    int found = 0;
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            char copy[256];
            strcpy(copy, line);
            char *id = strtok(copy, ",");
            if (!id) continue;
            if (strcmp(id, studentID) == 0) {
                found = 1;
                // remove newline
                line[strcspn(line, "\n")] = '\0';
                // duplicate checking
                char check[64];
				snprintf(check, sizeof(check), ",%s", courseID);
                if (strstr(line, check)) {
				    printf("Course already assigned!\n");
				    fprintf(temp, "%s\n", line);
				    continue;
				}
                // append new course
                fprintf(temp, "%s,%s\n", line, courseID);
            }
            else {
                fprintf(temp, "%s", line);
            }
        }
        fclose(f);
    }
    // student not present yet ? create new entry
    if (!found) {
        fprintf(temp,"%s,%s,%s,%s\n",studentID,student_name,student_dept,courseID);
    }
    fclose(temp);
    remove("data/student_courses.csv");
    rename("data/temp.csv", "data/student_courses.csv");
    printf("Course assigned successfully!\n");
    return 1;
}

void list_student_courses(const char *studentID) {
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) {
        printf("No course assignments found!\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id   = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        if (!id) continue;
        // --- compare student IDs ---
        if (strcmp(id, studentID) == 0) {
            found = 1;
            // Clean values
            if (!name) name = "";
            if (!dept) dept = "";
            dept[strcspn(dept, "\n")] = '\0';
            printf("\nStudent ID: %s\n", id);
            printf("Name: %s\n", name);
            printf("Department: %s\n", dept);
            printf("Assigned Courses: ");
            // remaining fields = courses
            char *course = strtok(NULL, ",");
            int first = 1;
            while (course) {
                course[strcspn(course, "\n")] = '\0';
                if (!first) printf(", ");
                printf("%s", course);
                first = 0;
                course = strtok(NULL, ",");
            }
            printf("\n");
            break;
        }
    }
    if (!found)
        printf("No courses assigned to ID %s\n", studentID);
    fclose(f);
}

int remove_course_from_student(const char *studentID, const char *courseID) {
    FILE *f = fopen("data/student_courses.csv", "r");
    if (!f) {
        printf("No course assignments file found!\n");
        return 0;
    }
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Failed to create temp file!\n");
        fclose(f);
        return 0;
    }
    char line[512];
    int found_student = 0, removed = 0;
    while (fgets(line, sizeof(line), f)) {
        char copy[512];
        strcpy(copy, line);
        char *id   = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *dept = strtok(NULL, ",");
        if (!id) continue;
        // clean fields
        if (!name) name = "";
        if (!dept) dept = "";
        name[strcspn(name, "\n")] = '\0';
        dept[strcspn(dept, "\n")] = '\0';
        // if this is NOT the student — copy unchanged
        if (strcmp(id, studentID) != 0) {
            fputs(line, temp);
            continue;
        }
        // This IS the student
        found_student = 1;
        // collect kept courses
        char *course = strtok(NULL, ",");
        char kept[50][50];
        int kept_count = 0;
        while (course) {
            course[strcspn(course, "\n")] = '\0';
            if (strcmp(course, courseID) == 0) {
                removed = 1;    // skip this course
            } else {
                strcpy(kept[kept_count++], course);
            }
            course = strtok(NULL, ",");
        }
        if (!removed) {
            printf("Course '%s' not found for this student.\n", courseID);
            fputs(line, temp);
            continue;
        }
        // Rebuild updated line
        fprintf(temp, "%s,%s,%s", id, name, dept);
        int i;
        for (i = 0; i < kept_count; i++) {
            fprintf(temp, ",%s", kept[i]);
        }
        fprintf(temp, "\n");
    }
    fclose(f);
    fclose(temp);
    remove("data/student_courses.csv");
    rename("data/temp.csv", "data/student_courses.csv");
    if (!found_student) {
        printf("Student with ID %s not found.\n", studentID);
        return 0;
    }
    if (!removed) return 0;
    printf("Course '%s' removed successfully!\n", courseID);
    return 1;
}


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
    char name[50] = "", dept[50] = "";
    // 1. Get teacher info
    if (!get_teacher_info(teacherID, name, dept)) {
        printf("Teacher not found!\n");
        return 0;
    }
    FILE *f = fopen("data/teacher_courses.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        printf("Cannot create temp file!\n");
        if (f) fclose(f);
        return 0;
    }
    char line[256];
    int found = 0;
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            line[strcspn(line, "\r\n")] = 0;  // remove newline
            char copy[256];
            strcpy(copy, line);
            char *tid = strtok(copy, ",");
            if (!tid) continue;
            tid[strcspn(tid, "\r\n")] = 0; // clean newline
            if (strcmp(tid, teacherID) == 0) {
            	if (!course_exists(courseID)) {
			    	printf("Course with ID %s does not exist!\n", courseID);
			    	return 0;
				}
                found = 1;
                // check duplicate
                if (strstr(line, courseID)) {
                    printf("Course '%s' already assigned to this teacher.\n",courseID);
                    fprintf(temp, "%s\n", line);
                    return 0;
                }
                fprintf(temp, "%s,%s,%s", tid, name, dept);
                // append old courses
                char *token = strtok(line, ","); // skip tid
                token = strtok(NULL, ",");       // skip name
                token = strtok(NULL, ",");       // skip dept
                token = strtok(NULL, ",");       // first course
                while (token) {
                    token[strcspn(token, "\r\n")] = 0;
                    fprintf(temp, ",%s", token);
                    token = strtok(NULL, ",");
                }
                // append new course
                fprintf(temp, ",%s\n", courseID);
            } else {
                fprintf(temp, "%s\n", line);
            }
        }
        fclose(f);
    }
    if (!found) {
        fprintf(temp, "%s,%s,%s,%s\n", teacherID, name, dept, courseID);
    }
    fclose(temp);
    remove("data/teacher_courses.csv");
    rename("data/temp.csv", "data/teacher_courses.csv");
    printf("Course '%s' assigned to teacher '%s' successfully!\n",courseID, teacherID);
    return 1;
}

void list_teacher_courses(const char *teacherID) {
    FILE *f = fopen("data/teacher_courses.csv", "r");
    if (!f) {
        printf("No course assignments found!\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
    	line[strcspn(line, "\r\n")] = 0;
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        if (!tid) continue;
        if (strcmp(tid, teacherID) == 0) {
            found = 1;
            char *name = strtok(NULL, ",");
            char *dept = strtok(NULL, ",");

            printf("\n%-18s %s\n", "Teacher ID:", teacherID);
			printf("%-18s %s\n", "Name:", name);
			printf("%-18s %s\n", "Department:", dept);
			printf("%-18s ",  "Assigned Courses:");
			
            char *course = strtok(NULL, ",");
            int first = 1;
            while (course) {
                course[strcspn(course, "\n")] = '\0';
                if (!first) printf(", ");
                printf("%s", course);
                first = 0;
                course = strtok(NULL, ",");
            }
            printf("\n");
        }
    }
    if (!found)
        printf("No courses assigned to this teacher.\n");
    fclose(f);
}

int remove_course_from_teacher(const char *teacherID, const char *courseID) {
    FILE *f = fopen("data/teacher_courses.csv", "r");
    FILE *temp = fopen("data/temp.csv", "w");
    if (!f || !temp) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        printf("Error opening files!\n");
        return 0;
    }
    char line[256];
    int foundTeacher = 0;
    int removed = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;  // remove newline
        char copy[256];
        strcpy(copy, line);
        char *tid = strtok(copy, ",");
        char *tname = strtok(NULL, ",");
        char *tdept = strtok(NULL, ",");
        if (!tid || !tname || !tdept) {
            fprintf(temp, "%s\n", line);
            continue;
        }
        tid[strcspn(tid, "\r\n")] = 0;
        tname[strcspn(tname, "\r\n")] = 0;
        tdept[strcspn(tdept, "\r\n")] = 0;
        if (strcmp(tid, teacherID) == 0) {
            foundTeacher = 1;
            char courses[10][50];  // max 10 courses per teacher
            int courseCount = 0;
            char *token = strtok(NULL, ",");
            while (token) {
                token[strcspn(token, "\r\n")] = 0;
                if (strcmp(token, courseID) != 0) {
                    strcpy(courses[courseCount++], token);
                } else {
                    removed = 1;
                }
                token = strtok(NULL, ",");
            }
            // write teacher line back
            fprintf(temp, "%s,%s,%s", tid, tname, tdept);
            int i;
            for (i = 0; i < courseCount; i++)
                fprintf(temp, ",%s", courses[i]);
            fprintf(temp, "\n");

        } else {
            fprintf(temp, "%s\n", line);
        }
    }
    fclose(f);
    fclose(temp);
    remove("data/teacher_courses.csv");
    rename("data/temp.csv", "data/teacher_courses.csv");
    if (!foundTeacher) {
        printf("Teacher with '%s' not found!\n", teacherID);
        return 0;
    }
    if (!removed) {
        printf("Course '%s' not found for teacher '%s'.\n",courseID, teacherID);
        return 0;
    }
    printf("Course '%s' removed from teacher '%s' successfully!\n",courseID,teacherID);
    return 1;
}



//Helper Functions:
/* helper: trim leading + trailing whitespace (modifies string in-place) */
static void trim(char *s) {
    if (!s) return;
    /* trim leading */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start)+1);
    /* trim trailing */
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) {
        s[len-1] = '\0';
        len--;
    }
}

int course_exists(const char *courseID) {
    FILE *f = fopen("data/courses.csv", "r");
    if (!f) {
        printf("Courses file not found!\n");
        return 0;
	}
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char copy[256];
        strcpy(copy, line);
        char *id = strtok(copy, ",");
        if (!id) continue;
        id[strcspn(id, "\n")] = '\0';
        if (strcmp(id, courseID) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}






