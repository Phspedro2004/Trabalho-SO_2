#define MIN_PRIORITY 5
#define MAX_PRIORITY 1

// Envelhecer tarefas que ainda não foram executadas
void aging();

// Adiciona tarefa pra fila
void add(char *name, int priority, int burst);

// Chama o escalonador
void schedule();
