/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int deadline;
    int wait_time;
    int start_time;
} Task;

#endif