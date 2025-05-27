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

// Insere uma nova tarefa na fila de acordo com a prioridade
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = strdup(name);
    task->priority = priority;
    task->burst = burst;
    task->deadline = 0; // Ignorado no RR_P
    task->wait_time = 0; // Ignorado neste escalonador

    insert(&taskList, task);
}

// Executa o escalonamento Round Robin
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

        delete(&taskList); // Remove a tarefa atual da fila

        if (current->burst > 0) {
            insert(&taskList, current); // Reinsere a tarefa se ainda há tempo de CPU
        } else {
            free(current->name);
            free(current);
        }
    }
}
