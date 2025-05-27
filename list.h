/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void insert_EDF(struct node **head, Task *newTask);
void delete(struct node **head);
void traverse(struct node *head);