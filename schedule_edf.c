#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#include "schedule_edf.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"

struct node *taskList = NULL;

// Insere tarefa na fila EDF
void add(char *name, int priority, int burst, int deadline) {
    Task *task = malloc(sizeof(Task));
    task->name = strdup(name);
    task->priority = priority;
    task->burst = burst;
    int current_time = timer_get_time();
    task->deadline = current_time + deadline;
    task->start_time = current_time;
    
    task->wait_time = 0;

    insert_EDF(&taskList, task);
}

// Executa o escalonamento EDF
void schedule() {
    timer_start();

    while (taskList != NULL) {
        Task *t = taskList->task;

        
        if (timer_get_time() > t->deadline) {
            printf("[EDF] Tarefa [%s] perdeu o prazo (%d < %d). Ignorada.\n", t->name, t->deadline, timer_get_time());
            delete(&taskList);
            free(t->name);
            free(t);
            continue;
        }

        int slice = (t->burst > QUANTUM) ? QUANTUM : t->burst;

        run(t, slice);
        while (!timer_flag_slice()) {
            usleep(10000);
        }

        int finish_time = timer_get_time();
        t->burst -= slice;
        delete(&taskList);

        if (t->burst > 0) {
            insert_EDF(&taskList, t);
        } else {
            int tempC = finish_time - t->start_time;
            printf("[EDF] Tarefa [%s] tempo de conclusão = %d\n", t->name, tempC);
            free(t->name);
            free(t);
        }
    }
}
