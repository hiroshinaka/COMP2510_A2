#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**TODO
 * Check for invalid GPA (negative, > 4.0) and it PRINTS the same number out (does not convert to float)
 * Floats with more than 3 decimal places are truncated to 3 decimal places
 * Check for invalid GPA (negative, 0.0 - 4.3)
 * Check for invalid TOEFL (negative, > 120)
 *
 */
struct Student {
    char firstName[50];
    char lastName[50];
    char birthMonth[4];
    int birthDay;
    int birthYear;
    float gpa;
    char status;
    int toefl;   // Only for international students, 0 for domestic
};
//Method to check for leap years
int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 1 : 0;
}

//Method to get days in month, returns 0 if invalid month
int getDaysInMonth(const char *month, int year) {
    if (strcmp(month, "Jan") == 0) return 31;
    if (strcmp(month, "Feb") == 0) return isLeapYear(year) ? 29 : 28;
    if (strcmp(month, "Mar") == 0) return 31;
    if (strcmp(month, "Apr") == 0) return 30;
    if (strcmp(month, "May") == 0) return 31;
    if (strcmp(month, "Jun") == 0) return 30;
    if (strcmp(month, "Jul") == 0) return 31;
    if (strcmp(month, "Aug") == 0) return 31;
    if (strcmp(month, "Sep") == 0) return 30;
    if (strcmp(month, "Oct") == 0) return 31;
    if (strcmp(month, "Nov") == 0) return 30;
    if (strcmp(month, "Dec") == 0) return 31;
    return 0; // Invalid month
}

//Method to validate date
int isValidDate(const char *month, int day, int year) {
    int daysInMonth = getDaysInMonth(month, year);
    return (day > 0 && day <= daysInMonth) ? 1 : 0;
}

//Method to parse students with validation for each input.
int parseStudentLine(const char *line, struct Student *student, FILE *outputFile) {
    // Initially, try parsing assuming the student is international with TOEFL score
    int parsedCount = sscanf(line, "%49s %49s %3s-%d-%d %f %c %d",
                             student->firstName,
                             student->lastName,
                             student->birthMonth,
                             &student->birthDay,
                             &student->birthYear,
                             &student->gpa,
                             &student->status,
                             &student->toefl);


    // If the parsing count is 7, student is domestic
    if (parsedCount == 7) {
        // Reparse without expecting TOEFL
        parsedCount = sscanf(line, "%49s %49s %3s-%d-%d %f %c",
                             student->firstName,
                             student->lastName,
                             student->birthMonth,
                             &student->birthDay,
                             &student->birthYear,
                             &student->gpa,
                             &student->status);
        // Set TOEFL score to 0 for domestic student
        if(parsedCount == 7) {
            student->toefl = 0;
        }
        //Capitalizes first letter of month
        student->birthMonth[0] = toupper(student->birthMonth[0]);
        student->birthMonth[1] = tolower(student->birthMonth[1]);
        student->birthMonth[2] = tolower(student->birthMonth[2]);

        //Validate Month
        if (!isValidDate(student->birthMonth, student->birthDay, student->birthYear)) {
            fprintf(outputFile, "Error: Invalid date\n");
            exit(-1);
        }
        //Validate GPA
        if (student->gpa < 0.0 || student->gpa > 4.3) {
            fprintf(outputFile, "Error: Invalid GPA\n");
            exit(-3);
        }
        //Validate Status
        if (student->status != 'D' && student->status != 'I') {
            fprintf(outputFile, "Error: Invalid Student status\n");
            exit(-4);
        }
        //Validate TOEFL
        if (student->status == 'I' && (student->toefl < 0 || student->toefl > 120)) {
            fprintf(outputFile, "Error: Invalid TOEFL score\n");
            exit(-5);
        }
    }
    return parsedCount;
}


//Method to convert month to number to compare months
int monthToNumber(const char *month) {
    if (strcmp(month, "Jan") == 0) return 1;
    if (strcmp(month, "Feb") == 0) return 2;
    if (strcmp(month, "Mar") == 0) return 3;
    if (strcmp(month, "Apr") == 0) return 4;
    if (strcmp(month, "May") == 0) return 5;
    if (strcmp(month, "Jun") == 0) return 6;
    if (strcmp(month, "Jul") == 0) return 7;
    if (strcmp(month, "Aug") == 0) return 8;
    if (strcmp(month, "Sep") == 0) return 9;
    if (strcmp(month, "Oct") == 0) return 10;
    if (strcmp(month, "Nov") == 0) return 11;
    if (strcmp(month, "Dec") == 0) return 12;
    return -1;
}

int compareStudents(const void *a, const void *b) {
    const struct Student *studentA = (const struct Student *)a;
    const struct Student *studentB = (const struct Student *)b;

    if (studentA->birthYear != studentB->birthYear) {
        return studentA->birthYear - studentB->birthYear;
    }

    int monthComparison = monthToNumber(studentA->birthMonth) - monthToNumber(studentB->birthMonth);
    if (monthComparison != 0) {
        return monthComparison;
    }

    if (studentA->birthDay != studentB->birthDay) {
        return studentA->birthDay - studentB->birthDay;
    }

    int lastNameComparison = strcmp(studentA->lastName, studentB->lastName);
    if (lastNameComparison != 0) {
        return lastNameComparison;
    }

    int firstNameComparison = strcmp(studentA->firstName, studentB->firstName);
    if (firstNameComparison != 0) {
        return firstNameComparison;
    }

    if (studentA->gpa != studentB->gpa) {
        return (int)(1000 * (studentA->gpa - studentB->gpa));
    }

    if (studentA->status != studentB->status) {
        return studentA->status - studentB->status;
    }

    if (studentA->status == 'I') {
        return studentA->toefl - studentB->toefl;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <option>\n", argv[0]);
        return 1;
    }

    int option = atoi(argv[3]);
    if (option < 1 || option > 3) {
        fprintf(stderr, "Invalid option: Choose 1, 2, or 3.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Could not open input file\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Failed to create the output file.\n");
        fclose(inputFile);
        return 1;
    }

    struct Student *students = NULL;

    int numberStudents = 0;
    const int maxStudents = 100000;
    students = (struct Student *)malloc(maxStudents * sizeof(struct Student));
    if (students == NULL) {
        fprintf(stderr, "Failed to allocate memory for students.\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }
    int lineNum = 0;
    char line[256];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        lineNum++;

        int parsedCount = parseStudentLine(line, &students[numberStudents], outputFile);

        // If parsedCount is not 7 or 8, the input format is invalid
        if (parsedCount != 7 && parsedCount != 8) {
            fprintf(outputFile, "Error: Invalid input format at line %d\n", lineNum);
            fclose(inputFile);
            fclose(outputFile);
            free(students);
            return 1;
        }

        // If status is 'I' but parsedCount is 7, the TOEFL score is missing
        if (students[numberStudents].status == 'I' && parsedCount == 7) {
            fprintf(outputFile, "Error: Missing TOEFL score for international student at line %d\n", lineNum);
            fclose(inputFile);
            fclose(outputFile);
            free(students);
            return 1;
        }

        numberStudents++;
    }


    qsort(students, numberStudents, sizeof(struct Student), compareStudents);

    for (int i = 0; i < numberStudents; i++) {
        if (option == 1 && students[i].status == 'I') {
            continue;
        }
        if (option == 2 && students[i].status == 'D') {
            continue;
        }
        fprintf(outputFile, "%s %s %s %d %d %.3f %c", students[i].firstName,
                students[i].lastName, students[i].birthMonth, students[i].birthDay,
                students[i].birthYear, students[i].gpa, students[i].status);
        if (students[i].status == 'I') {
            fprintf(outputFile, " %d", students[i].toefl);
        }
        fprintf(outputFile, "\n");
    }

    fclose(inputFile);
    fclose(outputFile);
    free(students);
    return 0;
}
