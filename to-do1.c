#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // For sleep()

#define MAX_TASKS 100
#define MAX_NAME_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    char deadline[20]; // Format: YYYY-MM-DD HH:MM
    char priority[10]; // High, Medium, or Low
    int reminded_60;   // Flag for 60-minute reminder
    int reminded_30;   // Flag for 30-minute reminder
    int reminded_10;   // Flag for 10-minute reminder
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

// Function prototypes
void addTask();
void viewTasks();
void deleteTask();
void sendNotification(const char *message);
void checkDeadlines();
int parseDeadline(const char *deadline_str, struct tm *deadline_tm);

int main() {
    int choice;
    printf("Starting Task Management Program...\n");

    while (1) {
        printf("\n--- Task Management ---\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Delete Task\n");
        printf("4. Exit\n");
        printf("Checking deadlines in the background...\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }

        checkDeadlines(); // Check deadlines in the loop
        sleep(1); // To simulate background checking
    }

    return 0;
}

void addTask() {
    if (task_count >= MAX_TASKS) {
        printf("Task limit reached. Cannot add more tasks.\n");
        return;
    }

    printf("Enter task name: ");
    getchar(); // Consume newline from the input buffer
    fgets(tasks[task_count].name, MAX_NAME_LENGTH, stdin);
    tasks[task_count].name[strcspn(tasks[task_count].name, "\n")] = '\0'; // Remove newline

    printf("Enter deadline (YYYY-MM-DD HH:MM): ");
    fgets(tasks[task_count].deadline, 20, stdin);
    tasks[task_count].deadline[strcspn(tasks[task_count].deadline, "\n")] = '\0'; // Remove newline

    printf("Enter priority (High, Medium, Low): ");
    fgets(tasks[task_count].priority, 10, stdin);
    tasks[task_count].priority[strcspn(tasks[task_count].priority, "\n")] = '\0'; // Remove newline

    tasks[task_count].reminded_60 = 0; // Reset 60-minute reminder flag
    tasks[task_count].reminded_30 = 0; // Reset 30-minute reminder flag
    tasks[task_count].reminded_10 = 0; // Reset 10-minute reminder flag
    task_count++;

    printf("Task added successfully.\n");

    // Immediate notification for added task
    char message[200];
    snprintf(message, sizeof(message), "Task '%s' (Priority: %s) added with deadline '%s'.", 
             tasks[task_count - 1].name, tasks[task_count - 1].priority, tasks[task_count - 1].deadline);
    sendNotification(message);
}

void viewTasks() {
    if (task_count == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n--- Tasks ---\n");
    for (int i = 0; i < task_count; i++) {
        printf("%d. %s (Deadline: %s, Priority: %s)\n", i + 1, tasks[i].name, tasks[i].deadline, tasks[i].priority);
    }
}

void deleteTask() {
    if (task_count == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    int task_num;
    printf("Enter the task number to delete: ");
    scanf("%d", &task_num);

    if (task_num < 1 || task_num > task_count) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = task_num - 1; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    task_count--;
    printf("Task deleted successfully.\n");
}

void sendNotification(const char *message) {
    char command[512];
    snprintf(command, sizeof(command), 
             "powershell -Command \"Add-Type -AssemblyName System.Windows.Forms; "
             "[System.Windows.Forms.MessageBox]::Show(\\\"%s\\\")\"", message);
    system(command);
}

void checkDeadlines() {
    time_t now = time(NULL); // Current time

    for (int i = 0; i < task_count; i++) {
        struct tm deadline_tm = {0};
        if (!parseDeadline(tasks[i].deadline, &deadline_tm)) {
            printf("Invalid deadline format for task '%s'. Skipping.\n", tasks[i].name);
            continue;
        }
        time_t deadline_time = mktime(&deadline_tm);

        double diff = difftime(deadline_time, now);

        // Check reminders based on priority
        if (strcmp(tasks[i].priority, "High") == 0) {
            // 60-minute reminder
            if (diff > 0 && diff <= 3600 && !tasks[i].reminded_60) {
                char message[200];
                snprintf(message, sizeof(message), "Reminder: High-priority task '%s' is due in 60 minutes!", tasks[i].name);
                sendNotification(message);
                tasks[i].reminded_60 = 1;
            }
        }

        if (strcmp(tasks[i].priority, "High") == 0 || strcmp(tasks[i].priority, "Medium") == 0) {
            // 30-minute reminder
            if (diff > 0 && diff <= 1800 && !tasks[i].reminded_30) {
                char message[200];
                snprintf(message, sizeof(message), "Reminder: Task '%s' is due in 30 minutes!", tasks[i].name);
                sendNotification(message);
                tasks[i].reminded_30 = 1;
            }
        }

        // 10-minute reminder for all priorities
        if (diff > 0 && diff <= 600 && !tasks[i].reminded_10) {
            char message[200];
            snprintf(message, sizeof(message), "Reminder: Task '%s' is due in 10 minutes!", tasks[i].name);
            sendNotification(message);
            tasks[i].reminded_10 = 1;
        }
    }
}

int parseDeadline(const char *deadline_str, struct tm *deadline_tm) {
    // Initialize struct tm
    memset(deadline_tm, 0, sizeof(struct tm));

    // Parse the deadline string into struct tm
    if (sscanf(deadline_str, "%4d-%2d-%2d %2d:%2d",
               &deadline_tm->tm_year, &deadline_tm->tm_mon, &deadline_tm->tm_mday,
               &deadline_tm->tm_hour, &deadline_tm->tm_min) != 5) {
        return 0; // Parsing failed
    }

    // Adjust struct tm fields
    deadline_tm->tm_year -= 1900; // Years since 1900
    deadline_tm->tm_mon -= 1;     // Months are 0-based
    return 1; // Parsing successful
}
