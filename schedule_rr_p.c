#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#include "schedule_rr_p.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"

#define MIN_PRIORITY 5
#define MAX_PRIORITY 1

struct node *taskList[MIN_PRIORITY] = { NULL };

// Insere uma nova tarefa na fila de acordo com a prioridade
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = strdup(name);
    task->priority = priority;
    task->burst = burst;
    task->deadline = 0; // Ignorado no RR_P
    task->wait_time = 0; // Ignorado neste escalonador

    insert(&taskList[priority - 1], task);
}

// Executa o escalonamento com Round Robin por prioridade
void schedule() {
    while (1) {
        int p;
        for (p = MAX_PRIORITY; p <= MIN_PRIORITY; p++) {
            if (taskList[p - 1] != NULL) {
                break;
            }
        }

        // Todas as filas estão vazias
        if (p > MIN_PRIORITY) {
            break;
        }

        Task *t = taskList[p - 1]->task;
        int slice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

        run(t, slice);
        t->burst -= slice;

        delete(&taskList[p - 1]);

        if (t->burst > 0) {
            insert(&taskList[p - 1], t);
        } else {
            free(t->name);
            free(t);
        }
    }
}
