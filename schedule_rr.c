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

// Cria e insere uma nova tarefa na fila RR
void add(char *name, int burst) {
    Task *tarefa = malloc(sizeof(Task));
    tarefa->name = strdup(name);
    tarefa->burst = burst;
    tarefa->priority = 0;     // Ignorado no Round Robin
    tarefa->deadline = 0;     // Ignorado neste método
    tarefa->wait_time = 0;    // Não utilizado aqui

    insert(&taskList, tarefa);
}

// Executa o escalonamento Round Robin
void schedule() {
    while (taskList != NULL) {
        Task *current = taskList->task;

        int fatia = (current->burst > QUANTUM) ? QUANTUM : current->burst;

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
