#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_LENGTH 256
#define DATA_FILE "tasks.dat"

/* ── Data model ───────────────────────────────────────────── */

typedef struct {
    int  id;
    char title[MAX_LENGTH];
    int  completed;  /* 0 = pending, 1 = done */
} Task;

/* ── Persistence ──────────────────────────────────────────── */

int load_tasks(Task tasks[]) {
    FILE *f = fopen(DATA_FILE, "rb");
    if (!f) return 0;
    int count = 0;
    while (fread(&tasks[count], sizeof(Task), 1, f) == 1 && count < MAX_TASKS)
        count++;
    fclose(f);
    return count;
}

void save_tasks(Task tasks[], int count) {
    FILE *f = fopen(DATA_FILE, "wb");
    if (!f) { printf("Error: could not save tasks.\n"); return; }
    fwrite(tasks, sizeof(Task), count, f);
    fclose(f);
}

/* ── Helpers ──────────────────────────────────────────────── */

int next_id(Task tasks[], int count) {
    int max = 0;
    for (int i = 0; i < count; i++)
        if (tasks[i].id > max) max = tasks[i].id;
    return max + 1;
}

int find_task(Task tasks[], int count, int id) {
    for (int i = 0; i < count; i++)
        if (tasks[i].id == id) return i;
    return -1;
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ── Commands ─────────────────────────────────────────────── */

void cmd_list(Task tasks[], int count) {
    if (count == 0) {
        printf("\n  No tasks yet. Add one with 'a'.\n\n");
        return;
    }

    printf("\n  %-4s  %-6s  %s\n", "ID", "Status", "Title");
    printf("  %-4s  %-6s  %s\n", "----", "------", "------------------------------");

    for (int i = 0; i < count; i++) {
        const char *status = tasks[i].completed ? "[done]" : "[ -- ]";
        printf("  %-4d  %-6s  %s\n", tasks[i].id, status, tasks[i].title);
    }
    printf("\n");
}

void cmd_add(Task tasks[], int *count) {
    if (*count >= MAX_TASKS) {
        printf("Task limit reached (%d).\n", MAX_TASKS);
        return;
    }
    printf("Task title: ");
    char title[MAX_LENGTH];
    fgets(title, MAX_LENGTH, stdin);
    title[strcspn(title, "\n")] = '\0';  /* strip newline */

    if (strlen(title) == 0) {
        printf("Title cannot be empty.\n");
        return;
    }

    Task t;
    t.id        = next_id(tasks, *count);
    t.completed = 0;
    strncpy(t.title, title, MAX_LENGTH - 1);
    t.title[MAX_LENGTH - 1] = '\0';

    tasks[(*count)++] = t;
    save_tasks(tasks, *count);
    printf("Added: \"%s\" (ID %d)\n", t.title, t.id);
}

void cmd_complete(Task tasks[], int count) {
    printf("Task ID to mark complete: ");
    int id;
    if (scanf("%d", &id) != 1) { clear_input(); return; }
    clear_input();

    int idx = find_task(tasks, count, id);
    if (idx == -1) { printf("No task with ID %d.\n", id); return; }
    if (tasks[idx].completed) { printf("Already done!\n"); return; }

    tasks[idx].completed = 1;
    save_tasks(tasks, count);
    printf("Completed: \"%s\"\n", tasks[idx].title);
}

void cmd_delete(Task tasks[], int *count) {
    printf("Task ID to delete: ");
    int id;
    if (scanf("%d", &id) != 1) { clear_input(); return; }
    clear_input();

    int idx = find_task(tasks, *count, id);
    if (idx == -1) { printf("No task with ID %d.\n", id); return; }

    char title[MAX_LENGTH];
    strncpy(title, tasks[idx].title, MAX_LENGTH);

    /* shift remaining tasks left */
    for (int i = idx; i < *count - 1; i++)
        tasks[i] = tasks[i + 1];
    (*count)--;

    save_tasks(tasks, *count);
    printf("Deleted: \"%s\"\n", title);
}

void cmd_clear_done(Task tasks[], int *count) {
    int new_count = 0;
    for (int i = 0; i < *count; i++)
        if (!tasks[i].completed)
            tasks[new_count++] = tasks[i];

    int removed = *count - new_count;
    *count = new_count;
    save_tasks(tasks, *count);
    printf("Removed %d completed task(s).\n", removed);
}

/* ── Main loop ────────────────────────────────────────────── */

void print_menu() {
    printf("  a  Add task\n");
    printf("  l  List tasks\n");
    printf("  c  Complete task\n");
    printf("  d  Delete task\n");
    printf("  x  Clear completed\n");
    printf("  q  Quit\n");
    printf("\n> ");
}

int main(void) {
    Task tasks[MAX_TASKS];
    int  count = load_tasks(tasks);

    printf("\n=== Todo CLI ===\n\n");

    char choice;
    do {
        print_menu();
        if (scanf(" %c", &choice) != 1) break;
        clear_input();
        printf("\n");

        switch (choice) {
            case 'a': cmd_add(tasks, &count);        break;
            case 'l': cmd_list(tasks, count);        break;
            case 'c': cmd_complete(tasks, count);    break;
            case 'd': cmd_delete(tasks, &count);     break;
            case 'x': cmd_clear_done(tasks, &count); break;
            case 'q': printf("Bye!\n\n");            break;
            default:  printf("Unknown command.\n");  break;
        }
    } while (choice != 'q');

    return 0;
}
