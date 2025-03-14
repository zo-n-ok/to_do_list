#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define TASK_LENGTH 100

typedef struct {
    char task[TASK_LENGTH];
    int completed;
    int priority;
    char dueDate[20];
} ToDo;

void addTask(ToDo tasks[], int *taskCount) {
    if (*taskCount >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }
    printf("Enter a new task: ");
    getchar();
    fgets(tasks[*taskCount].task, TASK_LENGTH, stdin);
    tasks[*taskCount].task[strcspn(tasks[*taskCount].task, "\n")] = '\0';

    printf("Enter task priority (1-5): ");
    scanf("%d", &tasks[*taskCount].priority);

    printf("Enter due date (YYYY-MM-DD): ");
    scanf("%s", tasks[*taskCount].dueDate);

    tasks[*taskCount].completed = 0;
    (*taskCount)++;
    printf("Task added successfully!\n");
}

int compareTasks(const void *a, const void *b) {
    ToDo *taskA = (ToDo *)a;
    ToDo *taskB = (ToDo *)b;
    if (taskA->completed != taskB->completed) {
        return taskA->completed - taskB->completed; // Incomplete tasks first
    }
    if (taskA->priority != taskB->priority) {
        return taskA->priority - taskB->priority; // Then sort by priority
    }
    return strcmp(taskA->dueDate, taskB->dueDate); // Lastly by due date
}

void sortTasks(ToDo tasks[], int taskCount) {
    qsort(tasks, taskCount, sizeof(ToDo), compareTasks);
    printf("Tasks sorted by completion, priority, and due date!\n");
}

void viewTasks(ToDo tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\nYour To-Do List:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. [%c] %s (Priority: %d, Due: %s)\n", i + 1, tasks[i].completed ? 'X' : ' ', tasks[i].task, tasks[i].priority, tasks[i].dueDate);
    }
}

void markTaskComplete(ToDo tasks[], int taskCount) {
    int taskNum;
    printf("Enter task number to mark complete: ");
    scanf("%d", &taskNum);
    if (taskNum < 1 || taskNum > taskCount) {
        printf("Invalid task number!\n");
    } else {
        tasks[taskNum - 1].completed = 1;
        printf("Task marked as complete!\n");
    }
}

void saveTasks(ToDo tasks[], int taskCount) {
    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL) {
        printf("Error saving tasks!\n");
        return;
    }
    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d|%s|%d|%s\n", tasks[i].completed, tasks[i].task, tasks[i].priority, tasks[i].dueDate);
    }
    fclose(file);
    printf("Tasks saved successfully!\n");
}

void loadTasks(ToDo tasks[], int *taskCount) {
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        printf("No saved tasks found.\n");
        return;
    }
    while (fscanf(file, "%d|%[^|]|%d|%s\n", &tasks[*taskCount].completed, tasks[*taskCount].task, &tasks[*taskCount].priority, tasks[*taskCount].dueDate) != EOF) {
        (*taskCount)++;
    }
    fclose(file);
    printf("Tasks loaded successfully!\n");
}

void editTask(ToDo tasks[], int taskCount) {
    int taskNum;
    printf("Enter task number to edit: ");
    scanf("%d", &taskNum);
    if (taskNum < 1 || taskNum > taskCount) {
        printf("Invalid task number!\n");
    } else {
        printf("Enter the new task description: ");
        getchar();
        fgets(tasks[taskNum - 1].task, TASK_LENGTH, stdin);
        tasks[taskNum - 1].task[strcspn(tasks[taskNum - 1].task, "\n")] = '\0';

        printf("Enter new priority (1-5): ");
        scanf("%d", &tasks[taskNum - 1].priority);

        printf("Enter new due date (YYYY-MM-DD): ");
        scanf("%s", tasks[taskNum - 1].dueDate);

        printf("Task updated successfully!\n");
    }
}

int main() {
    ToDo tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    loadTasks(tasks, &taskCount);

    do {
        printf("\nTo-Do List Menu:\n");
        printf("1. Add Task\n2. View Tasks\n3. Mark Task Complete\n4. Edit Task\n5. Save Tasks\n6. Sort Tasks\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(tasks, &taskCount);
                break;
            case 2:
                viewTasks(tasks, taskCount);
                break;
            case 3:
                markTaskComplete(tasks, taskCount);
                break;
            case 4:
                editTask(tasks, taskCount);
                break;
            case 5:
                saveTasks(tasks, taskCount);
                break;
            case 6:
                sortTasks(tasks, taskCount);
                break;
            case 7:
                saveTasks(tasks, taskCount);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 7);

    return 0;
}
