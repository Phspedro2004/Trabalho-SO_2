
#include <stdio.h>
#include "task.h"

// run the specified task for the following time slice
void run(Task *task, int quantum) {
    printf("Running task = [%s] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst, quantum);
}