#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include "schedule_rr.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"

struct node *taskList = NULL;

// Insere uma nova tarefa na fila
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = strdup(name);
    task->burst = burst;
    task->priority = 0;
    task->deadline = 0;
    task->wait_time = 0;

    insert(&taskList, task);
}

// Executa o Round Robin
void schedule() {
    while (taskList != NULL) {
        Task *current = taskList->task;

        int fatia;
        if (current->burst > QUANTUM) {
            fatia = QUANTUM;
        } else {
            fatia = current->burst;
        }

        run(current, fatia);
        current->burst -= fatia;

        delete(&taskList); 

        if (current->burst > 0) {
            insert(&taskList, current); 
        } else {
            free(current->name);
            free(current);
        }
    }
}
