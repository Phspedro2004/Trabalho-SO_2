///////////////////////////////////////////////
#define MIN_PRIORITY 5
#define MAX_PRIORITY 1

// Envelhece tarefas de baixa prioridade
void aging();

// Adiciona uma nova tarefa a fila
void add(char *name, int priority, int burst);

// Chama o escalonador
void schedule();
///////////////////////////////////////////////