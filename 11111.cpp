#include <stdio.h>
#include <stdlib.h>  // ����qsort���������ͷ�ļ�
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
    {"�ߵ���ѧ", 4},
    {"��ѧӢ��", 2},
    {"���Դ���", 3},
    {"���ݽṹ", 3},
    {"���˼�������ԭ��", 3},
    {"�й����ִ�ʷ��Ҫ", 3},
    {"����������", 0.5},
    {"����", 1}
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
    if (strcmp(input, "����") == 0) {
        return 1;
    }
    return 0;
}

void addStudent(Student students[], int *studentCount) {
    if (*studentCount >= MAX_STUDENTS) {
        printf("�޷���Ӹ���ѧ�����Ѵﵽ�������\n");
        return;
    }

    char name[NAME_LENGTH];
    int studentID;
    printf("����ѧ������ : ");
    scanf("%s", name);
    if (checkReturn(name)) return;

    int existingIndex = findStudentByName(students, *studentCount, name);
    if (existingIndex != -1) {
        printf("ѧ�������ظ���ѡ�����:\n");
        printf("1. ������������\n");
        printf("2. ����\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 2) {
            return;
        } else if (choice == 1) {
            printf("����ѧ��ѧ��: ");
            scanf("%d", &studentID);

            Student *student = &students[existingIndex];
            strcpy(student->name, name);
            student->studentID = studentID;

            for (int i = 0; i < subjectCount; i++) {
                printf("����%s�ĳɼ�: ", subjects[i].subjectName);
                char input[10];
                scanf("%s", input);
                if (checkReturn(input)) return;
                student->grades[i] = atof(input);
            }
            student->subjectCount = subjectCount;
            printf("ѧ�������Ѹ���\n");
            return;
        }
    }

    printf("����ѧ��ѧ��: ");
    scanf("%d", &studentID);

    Student newStudent;
    strcpy(newStudent.name, name);
    newStudent.studentID = studentID;

    for (int i = 0; i < subjectCount; i++) {
        printf("����%s�ĳɼ�: ", subjects[i].subjectName);
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
    printf("ѧ������: %s\n", student->name);
    printf("ѧ��ѧ��: %d\n", student->studentID);

    for (int i = 0; i < student->subjectCount; i++) {
        printf("  ѧ������: %s\n", subjects[i].subjectName);
        printf("  ѧ�Ƴɼ�: %.2f\n", student->grades[i]);
        printf("  ѧ��ѧ��: %.1f\n", subjects[i].credit);
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
    printf("����ѧ��ѧ��: ");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int studentID = atoi(input);

    int index = findStudentByID(students, studentCount, studentID);
    if (index != -1) {
        displayStudent(&students[index]);
    } else {
        printf("δ�ҵ�ѧ��Ϊ %d ��ѧ��\n", studentID);
    }
}

void deleteStudent(Student students[], int *studentCount) {
    char input[10];
    printf("����Ҫɾ����ѧ��ѧ��: ");
    scanf("%s", input);
    if (checkReturn(input)) return;
    int studentID = atoi(input);

    int index = findStudentByID(students, *studentCount, studentID);
    if (index != -1) {
        for (int i = index; i < *studentCount - 1; i++) {
            students[i] = students[i + 1];
        }
        (*studentCount)--;
        printf("ѧ����ɾ��\n");
    } else {
        printf("δ�ҵ�ѧ��Ϊ %d ��ѧ��\n", studentID);
    }
}

void saveStudentsToFile(const Student students[], int studentCount) {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        printf("�޷����ļ�����д��\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void loadStudentsFromFile(Student students[], int *studentCount) {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("�޷����ļ����ж�ȡ\n");
        return;
    }
    fread(studentCount, sizeof(int), 1, file);
    fread(students, sizeof(Student), *studentCount, file);
    fclose(file);
}

void addSubject() {
    if (subjectCount >= MAX_SUBJECTS) {
        printf("�޷���Ӹ���ѧ�ƣ��Ѵﵽ�������\n");
        return;
    }

    char subjectName[NAME_LENGTH];
    float credit;

    printf("����ѧ������: ");
    scanf("%s", subjectName);
    if (checkReturn(subjectName)) return;

    int existingIndex = findSubjectByName(subjectName);
    if (existingIndex != -1) {
        printf("ѧ�������ظ���ѡ�����:\n");
        printf("1. ������������\n");
        printf("2. ����\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 2) {
            return;
        } else if (choice == 1) {
            printf("����ѧ��ѧ��: ");
            char input[10];
            scanf("%s", input);
            if (checkReturn(input)) return;
            credit = atof(input);
            subjects[existingIndex].credit = credit;
            printf("ѧ�������Ѹ���\n");
            return;
        }
    }

    printf("����ѧ��ѧ��: ");
    char input[10];
    scanf("%s", input);
    if (checkReturn(input)) return;
    credit = atof(input);

    Subject newSubject;
    strcpy(newSubject.subjectName, subjectName);
    newSubject.credit = credit;

    subjects[subjectCount] = newSubject;
    subjectCount++;
    printf("ѧ�������\n");
}

void deleteSubject() {
    char subjectName[NAME_LENGTH];
    printf("����Ҫɾ����ѧ������: ");
    scanf("%s", subjectName);
    if (checkReturn(subjectName)) return;

    int index = findSubjectByName(subjectName);
    if (index != -1) {
        for (int i = index; i < subjectCount - 1; i++) {
            subjects[i] = subjects[i + 1];
        }
        subjectCount--;
        printf("ѧ����ɾ��\n");
    } else {
        printf("δ�ҵ�����Ϊ %s ��ѧ��\n", subjectName);
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
    printf("����ѧ��ѧ��: ");
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

        printf("ѧ������: %s\n", studentGPA.name);
        printf("ѧ��ѧ��: %d\n", studentGPA.studentID);
        printf("ƽ��ѧ�ּ���(GPA): %.2f\n", studentGPA.gpa);
        printf("����: %d\n", studentGPA.rank);
    } else {
        printf("δ�ҵ�ѧ��Ϊ %d ��ѧ��\n", studentID);
    }
}

void displayAllGPAs(const Student students[], int studentCount) {
    StudentGPA studentGPAs[MAX_STUDENTS];
    calculateGPAs(students, studentCount, studentGPAs);
    qsort(studentGPAs, studentCount, sizeof(StudentGPA), compareGPA);

    for (int i = 0; i < studentCount; i++) {
        studentGPAs[i].rank = i + 1;
        printf("ѧ������: %s\n", studentGPAs[i].name);
        printf("ѧ��ѧ��: %d\n", studentGPAs[i].studentID);
        printf("ƽ��ѧ�ּ���(GPA): %.2f\n", studentGPAs[i].gpa);
        printf("����: %d\n", studentGPAs[i].rank);
        printf("\n");
    }
}

void GPAQuery(const Student students[], int studentCount) {
    char input[10];
    printf("ѡ�����:\n");
    printf("1. ��ѯ����ѧ����ƽ��ѧ�ּ��������\n");
    printf("2. ��ѯ����ѧ����ƽ��ѧ�ּ���\n");
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
            printf("��Ч��ѡ�������ѡ��\n");
    }
}

void studentManagement(Student students[], int *studentCount) {
    char input[10];
    while (1) {
        printf("\nѧ������\n");
        printf("1. ���ѧ��\n");
        printf("2. ��ʾ����ѧ��\n");
        printf("3. ��ʾ����ѧ��\n");
        printf("4. ɾ��ѧ��\n");
        printf("5. �������˵�\n");
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
                printf("��Ч��ѡ�������ѡ��\n");
        }
    }
}

void subjectManagement() {
    char input[10];
    while (1) {
        printf("\nѧ�ƹ���\n");
        printf("1. ���ѧ��\n");
        printf("2. ɾ��ѧ��\n");
        printf("3. �������˵�\n");
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
                printf("��Ч��ѡ�������ѡ��\n");
        }
    }
}

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    char input[10];

    loadStudentsFromFile(students, &studentCount);

    while (1) {
        printf("\nѧ���ɼ�����ϵͳ\n");
        printf("��ǰѧ������: %d\n", studentCount);
        printf("1. ѧ������\n");
        printf("2. ѧ�ƹ���\n");
        printf("3. GPA ��ѯ������\n");
        printf("4. �˳�\n");
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
                printf("��Ч��ѡ�������ѡ��\n");
        }
    }

    return 0;
}

