/////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include "schedule_rr_p.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"
#define MAX_PRIORITY 1
#define MIN_PRIORITY 5


struct node *taskList[MIN_PRIORITY] = { NULL };

// Adiciona uma nova tarefa a fila
void add(char *name, int priority, int burst) {
   Task *newTask = malloc(sizeof(Task));
   newTask->name = strdup(name);
   newTask->priority = priority;
   newTask->burst = burst;
   newTask->deadline = 0; // Não utilizado neste método
   newTask->wait_time = 0; // Não utilizado neste método
   insert(&taskList[priority - 1], newTask);
}

// Chama o escalonador
void schedule() {
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
      t->burst -= slice;
      delete(&taskList[p - 1]); // Remove a primeira tarefa da fila

      if (t->burst > 0) {
         insert(&taskList[p - 1], t); // Insere no final da fila (caso ainda não tenha finalizado o burst)
      } else {
         free(t->name);
         free(t);
      }
   }
}
/////////////////////////////////////////////////////////////////