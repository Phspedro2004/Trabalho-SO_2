
#define MIN_PRIORITY 1
#define MAX_PRIORITY 5

// add a task to the list 
void add(char *name, int priority, int burst, int deadline);

// invoke the scheduler
void schedule();
