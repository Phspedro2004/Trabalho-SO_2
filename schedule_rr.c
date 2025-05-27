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

// Adiciona uma nova tarefa a fila
void add(char *name, int burst){
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->burst = burst;
    newTask->priority = 0; // Não utilizado neste método
    newTask->deadline = 0; // Não utilizado neste método
    newTask->wait_time = 0; // Não utilizado neste método
    insert(&taskList, newTask);
}

// Chama o escalonador
void schedule(){
    timer_start();
    while (taskList != NULL) { // Enquanto a fila não está vazia
        Task *t = taskList->task;
        int slice;
        if (t->burst > QUANTUM) {
            slice = QUANTUM;
        } else {
            slice = t->burst;
        }
        
        run(t, slice);
        while (!timer_flag_slice()) {
            // usleep(1);
        }
        t->burst -= slice;
        delete(&taskList); // Remove a primeira tarefa da fila
        
        if (t->burst > 0) {
            insert(&taskList, t); // Insere no final da fila (caso ainda não tenha finalizado o burst)
        } else {
            free(t->name);
            free(t);
        }
    }
}