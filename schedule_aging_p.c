#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#include "schedule_aging_p.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"

#define AGING_LIMIT 5

struct node *taskList[MIN_PRIORITY] = { NULL };

// Aplica envelhecimento nas tarefas de menor prioridade
void aging() {
    for (int prio = MIN_PRIORITY - 1; prio >= MAX_PRIORITY - 1; prio--) {
        struct node *curr = taskList[prio];
        struct node *prev = NULL;

        while (curr != NULL) {
            Task *t = curr->task;
            t->wait_time++;

            if (prio > 0 && t->wait_time >= AGING_LIMIT) {
                struct node *promote = curr;

                if (prev == NULL) {
                    taskList[prio] = curr->next;
                    curr = taskList[prio];
                } else {
                    prev->next = curr->next;
                    curr = curr->next;
                }

                promote->next = NULL;
                t->priority--;
                t->wait_time = 0;
                insert(&taskList[prio - 1], t);
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

// Adiciona uma tarefa à fila de acordo com a prioridade
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = strdup(name);
    task->priority = priority;
    task->burst = burst;
    task->wait_time = 0;
    task->deadline = 0;

    insert(&taskList[priority - 1], task);
}

// Executa o escalonador com política de envelhecimento por prioridade
void schedule() {
    timer_start();
    int counter = 0;

    while (1) {
        int p;
        for (p = MAX_PRIORITY; p <= MIN_PRIORITY; p++) {
            if (taskList[p - 1] != NULL)
                break;
        }

        if (p > MIN_PRIORITY)
            break;

        Task *t = taskList[p - 1]->task;
        int timeSlice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

        run(t, timeSlice);

        while (!timer_flag_slice()) {
            usleep(1);
        }

        t->burst -= timeSlice;
        delete(&taskList[p - 1]);

        if (t->burst > 0) {
            insert(&taskList[p - 1], t);
        } else {
            free(t->name);
            free(t);
        }

        counter++;
        if (counter >= 5) {
            aging();
            counter = 0;
        }
    }
}
