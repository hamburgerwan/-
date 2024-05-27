#include <stdio.h>
#include <stdlib.h>  // 包含qsort函数所需的头文件
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 20
#define NAME_LENGTH 50
#define FILE_NAME "students.dat"
#define SUBJECT_COUNT 8

typedef struct {
    char subjectName[NAME_LENGTH];
    float credit;
} Subject;

typedef struct {
    char name[NAME_LENGTH];
    int studentID;
    float grades[MAX_SUBJECTS];
    int subjectCount;
} Student;

typedef struct {
    char name[NAME_LENGTH];
    int studentID;
    float gpa;
    int rank;
} StudentGPA;

Subject subjects[MAX_SUBJECTS] = {
    {"高等数学", 4},
    {"大学英语", 2},
    {"线性代数", 3},
    {"数据结构", 3},
    {"马克思主义基本原理", 3},
    {"中国近现代史纲要", 3},
    {"形势与政策", 0.5},
    {"体育", 1}
};
int subjectCount = SUBJECT_COUNT;

int findStudentByName(const Student students[], int studentCount, const char *name) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int findSubjectByName(const char *name) {
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i].subjectName, name) == 0) {
            return i;
        }
    }
    return -1;
}

int checkReturn(char *input) {
    if (strcmp(input, "返回") == 0) {
        return 1;
    }
    return 0;
}

void addStudent(Student students[], int *studentCount) {
    if (*studentCount >= MAX_STUDENTS) {
        printf("无法添加更多学生，已达到最大数量\n");
        return;
    }

    char name[NAME_LENGTH];
    int studentID;
    printf("输入学生姓名 : ");
    scanf("%s", name);
    if (checkReturn(name)) return;

    int existingIndex = findStudentByName(students, *studentCount, name);
    if (existingIndex != -1) {
        printf("学生姓名重复，选择操作:\n");
        printf("1. 覆盖现有数据\n");
        printf("2. 返回\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 2) {
            return;
        } else if (choice == 1) {
            printf("输入学生学号: ");
            scanf("%d", &studentID);

            Student *student = &students[existingIndex];
            strcpy(student->name, name);
            student->studentID = studentID;

            for (int i = 0; i < subjectCount; i++) {
                printf("输入%s的成绩: ", subjects[i].subjectName);
                char input[10];
                scanf("%s", input);
                if (checkReturn(input)) return;
                student->grades[i] = atof(input);
            }
            student->subjectCount = subjectCount;
            printf("学生数据已覆盖\n");
            return;
        }
    }

    printf("输入学生学号: ");
    scanf("%d", &studentID);

    Student newStudent;
    strcpy(newStudent.name, name);
    newStudent.studentID = studentID;

    for (int i = 0; i < subjectCount; i++) {
        printf("输入%s的成绩: ", subjects[i].subjectName);
        char input[10];
        scanf("%s", input);
        if (checkReturn(input)) return;
        newStudent.grades[i] = atof(input);
    }
    newStudent.subjectCount = subjectCount;

    students[*studentCount] = newStudent;
    (*studentCount)++;
}

void displayStudent(const Student *student) {
    printf("学生姓名: %s\n", student->name);
    printf("学生学号: %d\n", student->studentID);

    for (int i = 0; i < student->subjectCount; i++) {
        printf("  学科名称: %s\n", subjects[i].subjectName);
        printf("  学科成绩: %.2f\n", student->grades[i]);
        printf("  学科学分: %.1f\n", subjects[i].credit);
    }
}

void displayStudents(const Student students[], int studentCount) {
    for (int i = 0; i < studentCount; i++) {
        displayStudent(&students[i]);
    }
}

int findStudentByID(const Student students[], int studentCount, int studentID) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentID == studentID) {
            return i;
        }
    }
    return -1;
}

void displayStudentByID(const Student students[], int studentCount) {
    char input[10];
    printf("输入学生学号: ");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int studentID = atoi(input);

    int index = findStudentByID(students, studentCount, studentID);
    if (index != -1) {
        displayStudent(&students[index]);
    } else {
        printf("未找到学号为 %d 的学生\n", studentID);
    }
}

void deleteStudent(Student students[], int *studentCount) {
    char input[10];
    printf("输入要删除的学生学号: ");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int studentID = atoi(input);

    int index = findStudentByID(students, *studentCount, studentID);
    if (index != -1) {
        for (int i = index; i < *studentCount - 1; i++) {
            students[i] = students[i + 1];
        }
        (*studentCount)--;
        printf("学生已删除\n");
    } else {
        printf("未找到学号为 %d 的学生\n", studentID);
    }
}

void saveStudentsToFile(const Student students[], int studentCount) {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        printf("无法打开文件进行写入\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void loadStudentsFromFile(Student students[], int *studentCount) {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("无法打开文件进行读取\n");
        return;
    }
    fread(studentCount, sizeof(int), 1, file);
    fread(students, sizeof(Student), *studentCount, file);
    fclose(file);
}

void addSubject() {
    if (subjectCount >= MAX_SUBJECTS) {
        printf("无法添加更多学科，已达到最大数量\n");
        return;
    }

    char subjectName[NAME_LENGTH];
    float credit;

    printf("输入学科名称: ");
    scanf("%s", subjectName);
    if (checkReturn(subjectName)) return;

    int existingIndex = findSubjectByName(subjectName);
    if (existingIndex != -1) {
        printf("学科名称重复，选择操作:\n");
        printf("1. 覆盖现有数据\n");
        printf("2. 返回\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 2) {
            return;
        } else if (choice == 1) {
            printf("输入学科学分: ");
            char input[10];
            scanf("%s", input);
            if (checkReturn(input)) return;
            credit = atof(input);
            subjects[existingIndex].credit = credit;
            printf("学科数据已覆盖\n");
            return;
        }
    }

    printf("输入学科学分: ");
    char input[10];
    scanf("%s", input);
    if (checkReturn(input)) return;
    credit = atof(input);

    Subject newSubject;
    strcpy(newSubject.subjectName, subjectName);
    newSubject.credit = credit;

    subjects[subjectCount] = newSubject;
    subjectCount++;
    printf("学科已添加\n");
}

void deleteSubject() {
    char subjectName[NAME_LENGTH];
    printf("输入要删除的学科名称: ");
    scanf("%s", subjectName);
    if (checkReturn(subjectName)) return;

    int index = findSubjectByName(subjectName);
    if (index != -1) {
        for (int i = index; i < subjectCount - 1; i++) {
            subjects[i] = subjects[i + 1];
        }
        subjectCount--;
        printf("学科已删除\n");
    } else {
        printf("未找到名称为 %s 的学科\n", subjectName);
    }
}

float calculateGPA(const Student *student) {
    float totalGradePoints = 0.0;
    float totalCredits = 0.0;

    for (int i = 0; i < student->subjectCount; i++) {
        float grade = student->grades[i] < 60 ? 0 : student->grades[i];
        totalGradePoints += grade * subjects[i].credit;
        totalCredits += subjects[i].credit;
    }

    float gpa = (totalGradePoints / totalCredits) / 10 - 5;
    return gpa;
}

void calculateGPAs(const Student students[], int studentCount, StudentGPA studentGPAs[]) {
    for (int i = 0; i < studentCount; i++) {
        studentGPAs[i].gpa = calculateGPA(&students[i]);
        strcpy(studentGPAs[i].name, students[i].name);
        studentGPAs[i].studentID = students[i].studentID;
    }
}

int compareGPA(const void *a, const void *b) {
    float gpaA = ((StudentGPA *)a)->gpa;
    float gpaB = ((StudentGPA *)b)->gpa;

    if (gpaA < gpaB) return 1;
    if (gpaA > gpaB) return -1;
    return 0;
}

void displayGPAByID(const Student students[], int studentCount) {
    char input[10];
    printf("输入学生学号: ");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int studentID = atoi(input);

    int index = findStudentByID(students, studentCount, studentID);
    if (index != -1) {
        StudentGPA studentGPA;
        studentGPA.gpa = calculateGPA(&students[index]);
        strcpy(studentGPA.name, students[index].name);
        studentGPA.studentID = students[index].studentID;
        
        StudentGPA studentGPAs[MAX_STUDENTS];
        calculateGPAs(students, studentCount, studentGPAs);
        qsort(studentGPAs, studentCount, sizeof(StudentGPA), compareGPA);

        for (int i = 0; i < studentCount; i++) {
            if (studentGPAs[i].studentID == studentID) {
                studentGPA.rank = i + 1;
                break;
            }
        }

        printf("学生姓名: %s\n", studentGPA.name);
        printf("学生学号: %d\n", studentGPA.studentID);
        printf("平均学分绩点(GPA): %.2f\n", studentGPA.gpa);
        printf("排名: %d\n", studentGPA.rank);
    } else {
        printf("未找到学号为 %d 的学生\n", studentID);
    }
}

void displayAllGPAs(const Student students[], int studentCount) {
    StudentGPA studentGPAs[MAX_STUDENTS];
    calculateGPAs(students, studentCount, studentGPAs);
    qsort(studentGPAs, studentCount, sizeof(StudentGPA), compareGPA);

    for (int i = 0; i < studentCount; i++) {
        studentGPAs[i].rank = i + 1;
        printf("学生姓名: %s\n", studentGPAs[i].name);
        printf("学生学号: %d\n", studentGPAs[i].studentID);
        printf("平均学分绩点(GPA): %.2f\n", studentGPAs[i].gpa);
        printf("排名: %d\n", studentGPAs[i].rank);
        printf("\n");
    }
}

void GPAQuery(const Student students[], int studentCount) {
    char input[10];
    printf("选择操作:\n");
    printf("1. 查询单个学生的平均学分绩点和排名\n");
    printf("2. 查询所有学生的平均学分绩点\n");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int choice = atoi(input);

    switch (choice) {
        case 1:
            displayGPAByID(students, studentCount);
            break;
        case 2:
            displayAllGPAs(students, studentCount);
            break;
        default:
            printf("无效的选项，请重新选择\n");
    }
}

void studentManagement(Student students[], int *studentCount) {
    char input[10];
    while (1) {
        printf("\n学生管理\n");
        printf("1. 添加学生\n");
        printf("2. 显示所有学生\n");
        printf("3. 显示单个学生\n");
        printf("4. 删除学生\n");
        printf("5. 返回主菜单\n");
        scanf("%s", input);
        if (checkReturn(input)) return;
        int choice = atoi(input);

        switch (choice) {
            case 1:
                addStudent(students, studentCount);
                break;
            case 2:
                displayStudents(students, *studentCount);
                break;
            case 3:
                displayStudentByID(students, *studentCount);
                break;
            case 4:
                deleteStudent(students, studentCount);
                break;
            case 5:
                return;
            default:
                printf("无效的选项，请重新选择\n");
        }
    }
}

void subjectManagement() {
    char input[10];
    while (1) {
        printf("\n学科管理\n");
        printf("1. 添加学科\n");
        printf("2. 删除学科\n");
        printf("3. 返回主菜单\n");
        scanf("%s", input);
        if (checkReturn(input)) return;
        int choice = atoi(input);

        switch (choice) {
            case 1:
                addSubject();
                break;
            case 2:
                deleteSubject();
                break;
            case 3:
                return;
            default:
                printf("无效的选项，请重新选择\n");
        }
    }
}

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    char input[10];

    loadStudentsFromFile(students, &studentCount);

    while (1) {
        printf("\n学生成绩管理系统\n");
        printf("当前学生总数: %d\n", studentCount);
        printf("1. 学生管理\n");
        printf("2. 学科管理\n");
        printf("3. GPA 查询与排名\n");
        printf("4. 退出\n");
        scanf("%s", input);
        if (checkReturn(input)) continue;
        int choice = atoi(input);

        switch (choice) {
            case 1:
                studentManagement(students, &studentCount);
                break;
            case 2:
                subjectManagement();
                break;
            case 3:
                GPAQuery(students, studentCount);
                break;
            case 4:
                saveStudentsToFile(students, studentCount);
                return 0;
            default:
                printf("无效的选项，请重新选择\n");
        }
    }

    return 0;
}

