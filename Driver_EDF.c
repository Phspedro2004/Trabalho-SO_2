/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"

#include "schedule_edf.h"


#define SIZE 100

int main(int argc, char *argv[])
{
    FILE *in;

    char *temp;
    char task[SIZE];

    char *name;
    int burst;
    int priority; 
    int deadline; 

    in = fopen(argv[1],"r");
    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        burst = atoi(strsep(&temp,","));
        priority = atoi(strsep(&temp,",")); 
        deadline = atoi(strsep(&temp, ",")); 

        add(name,priority,burst, deadline);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}