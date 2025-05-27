/**
 * Funções auxiliares para manipulação de listas encadeadas
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "list.h"
#include "task.h"

// Insere uma nova tarefa no final da lista
void insert(struct node **head, Task *task) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = task;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct node *nav = *head;
        while (nav->next != NULL) {
            nav = nav->next;
        }
        nav->next = newNode;
    }
}

// Insere uma nova tarefa em ordem crescente de deadline (para EDF)
void insert_EDF(struct node **head, Task *task) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = task;
    newNode->next = NULL;

    if (*head == NULL || task->deadline < (*head)->task->deadline) {
        newNode->next = *head;
        *head = newNode;
    } else {
        struct node *nav = *head;
        while (nav->next != NULL && nav->next->task->deadline <= task->deadline) {
            nav = nav->next;
        }
        newNode->next = nav->next;
        nav->next = newNode;
    }
}

// Remove a tarefa do início da lista
void delete(struct node **head) {
    if (*head == NULL)
        return;

    struct node *temp = *head;
    *head = (*head)->next;
    free(temp);
}

// Percorre a lista e imprime cada tarefa
void traverse(struct node *head) {
    struct node *temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n", temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}