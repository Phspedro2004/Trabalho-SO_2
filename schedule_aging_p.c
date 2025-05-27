#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include "schedule_aging_p.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"
#define AGING_LIMIT 2


struct node *taskList[MIN_PRIORITY] = { NULL };

// Envelhece tarefas de baixa prioridade
void aging() {
    for (int p = MIN_PRIORITY - 1; p >= MAX_PRIORITY - 1; p--) {
        struct node *nav = taskList[p];
        struct node *prev = NULL;

        while (nav != NULL) {
            Task *t = nav->task;
            t->wait_time++; // Aumenta o tempo de espera

            if (p > 0 && t->wait_time >= AGING_LIMIT) { // Se atingir limite de espera, aumenta a prioridade
                struct node *up = nav; // Tarefa a ter prioridade aumentada

                if (prev == NULL) { // Se é o primeiro nó da fila
                    taskList[p] = nav->next;
                    nav = taskList[p]; // Atualiza o nav pois segue o loop
                } else {
                    prev->next = nav->next;
                    nav = nav->next;
                }

                up->next = NULL;
                t->priority--; // Aumenta a prioridade
                t->wait_time = 0; // Reinicia a contagem do tempo de espera
                insert(&taskList[p - 1], t); // Insere no fim da fila superior
            } else {
                prev = nav;
                nav = nav->next;
            }
        }
    }
}

// Adiciona uma nova tarefa a fila
void add(char *name, int priority, int burst){
   Task *newTask = malloc(sizeof(Task));
   newTask->name = strdup(name);
   newTask->priority = priority;
   newTask->burst = burst;
   newTask->wait_time = 0;
   newTask->deadline = 0; // Não utilizado neste método
   insert(&taskList[priority - 1], newTask);
}

// Chama o escalonador
void schedule(){
    timer_start();
    int aging_counter = 0;

    while (1) {
        int p;
        for (p = MAX_PRIORITY; p <= MIN_PRIORITY; p++) { // Encontra a fila de maior prioridade não vazia
            if (taskList[p - 1] != NULL) 
                break;
        }
        if (p > MIN_PRIORITY) { // Se todas filas vazia
            break;
        }

        int slice;
        Task *t = taskList[p - 1]->task;
        if (t->burst > QUANTUM) {
            slice = QUANTUM;
        } else {
            slice = t->burst;
        }

        run(t, slice);
        while (!timer_flag_slice()) {
            usleep(1);
        }
        t->burst -= slice;
        delete(&taskList[p - 1]); // Remove a primeira tarefa da fila

        if (t->burst > 0) {
            insert(&taskList[p - 1], t); // Insere no final da fila (caso ainda não tenha finalizado o burst)
        } else {
            free(t->name);
            free(t);
        }
        
        aging_counter++;
        if (aging_counter >= 10) {
            aging(); // Envelhecimento a cada ciclo
            aging_counter = 0;
        }
    }
}