#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

struct Employee {
    int id;
    char name[50];
    float salary;
    char joinDate[20];
};

// Function Prototypes
void addEmployee();
void displayEmployees();
void searchEmployee();
void deleteEmployee();
int getNextEmployeeID();
void getCurrentDateTime(char* dateStr);

int main() {
    MessageBoxW(NULL, L"Welcome to Employee Management System", L"Start", MB_OK | MB_ICONINFORMATION);
    int choice;

    do {
        printf("\n============================================\n");
        printf("       Employee Management System          \n");
        printf("============================================\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Exit\n");
        printf("============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: addEmployee(); break;
        case 2: displayEmployees(); break;
        case 3: searchEmployee(); break;
        case 4: deleteEmployee(); break;
        case 5:
            MessageBoxW(NULL, L"Exiting the program...", L"Exit", MB_OK | MB_ICONEXCLAMATION);
            exit(0);
        default: printf("Invalid choice, try again!\n");
        }
    } while (1);

    return 0;
}

// Function to Get Current Date and Time
void getCurrentDateTime(char* dateStr) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d-%02d-%04d %02d:%02d:%02d",
        tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// Function to Get Next Employee ID
int getNextEmployeeID() {
    FILE* file = fopen("employees.txt", "r");
    if (!file) return 1; // If file doesn't exist, start with ID 1

    int id = 0, lastID = 0;
    char name[50], date[20];
    float salary;

    while (fscanf(file, "%d %[^\t] %f %[^\n]", &id, name, &salary, date) == 4) {
        lastID = id;
    }
    fclose(file);
    return lastID + 1; // Increment the last used ID
}

// Function to Add Employee
void addEmployee() {
    FILE* file = fopen("employees.txt", "a");
    if (!file) {
        MessageBoxW(NULL, L"Error opening file!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    struct Employee emp;
    emp.id = getNextEmployeeID(); // Auto-incremented ID
    printf("\nEnter Employee Name: ");
    scanf(" %[^\n]", emp.name);
    printf("Enter Employee Salary: ");
    scanf("%f", &emp.salary);

    getCurrentDateTime(emp.joinDate); // Auto-set joining date

    fprintf(file, "%d %s %.2f %s\n", emp.id, emp.name, emp.salary, emp.joinDate);
    fclose(file);

    MessageBoxW(NULL, L"Employee Added Successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
}

// Function to Display Employees in a Proper Table Format
void displayEmployees() {
    FILE* file = fopen("employees.txt", "r");
    if (!file) {
        MessageBoxW(NULL, L"No employees found!", L"Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    struct Employee emp;
    int records = 0;

    printf("\n===============================================================\n");
    printf("| %-5s | %-20s | %-10s | %-19s |\n", "ID", "Name", "Salary", "Joining Date & Time");
    printf("---------------------------------------------------------------\n");

    while (fscanf(file, "%d %[^\t] %f %[^\n]", &emp.id, emp.name, &emp.salary, emp.joinDate) == 4) {
        printf("| %-5d | %-20s | %-10.2f | %-19s |\n", emp.id, emp.name, emp.salary, emp.joinDate);
        records++;
    }

    if (records == 0) {
        printf("|                  No Employees Found!                        |\n");
    }

    printf("===============================================================\n");

    fclose(file);
}

// Function to Search Employee by ID
void searchEmployee() {
    FILE* file = fopen("employees.txt", "r");
    if (!file) {
        MessageBoxW(NULL, L"No employees found!", L"Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    int id, found = 0;
    struct Employee emp;
    printf("\nEnter Employee ID to search: ");
    scanf("%d", &id);

    while (fscanf(file, "%d %[^\t] %f %[^\n]", &emp.id, emp.name, &emp.salary, emp.joinDate) == 4) {
        if (emp.id == id) {
            printf("\n====================================\n");
            printf("| %-10s | %-20s |\n", "Field", "Value");
            printf("------------------------------------\n");
            printf("| %-10s | %-20d |\n", "ID", emp.id);
            printf("| %-10s | %-20s |\n", "Name", emp.name);
            printf("| %-10s | %-20.2f |\n", "Salary", emp.salary);
            printf("| %-10s | %-20s |\n", "Join Date", emp.joinDate);
            printf("====================================\n");
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        MessageBoxW(NULL, L"Employee Not Found!", L"Error", MB_OK | MB_ICONERROR);
    }
}

// Function to Delete an Employee
void deleteEmployee() {
    FILE* file = fopen("employees.txt", "r");
    if (!file) {
        MessageBoxW(NULL, L"No employees found!", L"Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        MessageBoxW(NULL, L"Error creating temp file!", L"Error", MB_OK | MB_ICONERROR);
        fclose(file);
        return;
    }

    int id, found = 0;
    struct Employee emp;
    printf("\nEnter Employee ID to delete: ");
    scanf("%d", &id);

    while (fscanf(file, "%d %[^\t] %f %[^\n]", &emp.id, emp.name, &emp.salary, emp.joinDate) == 4) {
        if (emp.id == id) {
            found = 1; // Skip this employee (delete it)
        }
        else {
            fprintf(temp, "%d %s %.2f %s\n", emp.id, emp.name, emp.salary, emp.joinDate);
        }
    }
    fclose(file);
    fclose(temp);

    if (remove("employees.txt") != 0 || rename("temp.txt", "employees.txt") != 0) {
        MessageBoxW(NULL, L"Error updating file!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (found) {
        MessageBoxW(NULL, L"Employee Deleted Successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBoxW(NULL, L"Employee Not Found!", L"Error", MB_OK | MB_ICONERROR);
    }
}