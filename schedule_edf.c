/////////////////////////////////////////////////////////////////
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

// Adiciona uma nova tarefa ao EDF (em ordem de deadline)
void add(char *name, int priority, int burst, int deadline){
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    int now = timer_get_time();
    newTask->deadline = now + deadline; // Relativo
    newTask->start_time = now; // Timestamp de chegada
    newTask->wait_time = 0;
    insert_EDF(&taskList, newTask);
}

// Chama o escalonador
void schedule(){
    timer_start();   
    while (taskList != NULL) {
        Task *t = taskList->task;

        // Verifica se a tarefa perdeu o deadline
        if (timer_get_time() > t->deadline) {
            printf("[EDF] Task [%s] perdeu o deadline (%d < %d). Descartada.\n", t->name, t->deadline, timer_get_time());
            delete(&taskList);
            free(t->name);
            free(t);
            continue;
        }

        int slice;
        if (t->burst > QUANTUM) {
            slice = QUANTUM;
        } else {
            slice = t->burst;
        }

        run(t, slice);
        while (!timer_flag_slice()) {
            usleep(10000);
        }
        int end_time = timer_get_time();

        t->burst -= slice;
        delete(&taskList); // Remove a primeira tarefa da fila

        if (t->burst > 0) {
            insert_EDF(&taskList, t); // Insere na fila
        } else {
            int turnaround = end_time - t->start_time;
            printf("[EDF] Task [%s] turnaround = %d\n", t->name, turnaround);
            free(t->name);
            free(t);
        }
    }
   
}
/////////////////////////////////////////////////////////////////