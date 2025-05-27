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
#include "schedule_rr_p.h"
//#include "schedule_rr.h"
//#include "schedule_edf.h"
//#include "schedule_aging_p.h"

#define SIZE 100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    //int deadline;

    in = fopen(argv[1],"r");
    
    //timer_start();
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        burst = atoi(strsep(&temp,","));
        
        //EDF - RR_P
        priority = atoi(strsep(&temp,","));
        
        //EDF 
        //deadline = atoi(strsep(&temp, ","));

        //ADDS
        //RR
        // add(name, burst); 

        //RR_Prioridade
         add(name,priority,burst);

        //EDF
        //add(name,priority,burst, deadline);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}