#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"
#include "CPU.h"

static int tempo_atual = 0;
static bool flag_tempo = false;
pthread_mutex_t lock_timer = PTHREAD_MUTEX_INITIALIZER;

// Função da thread que simula o temporizador de hardware
void *temporizador_thread() {
    while (1) {
        usleep(10000); // cada unidade de tempo simulada

        pthread_mutex_lock(&lock_timer);
        tempo_atual++;

        if (tempo_atual % QUANTUM == 0) {
            flag_tempo = true;
        }
        pthread_mutex_unlock(&lock_timer);
    }
    return NULL;
}

// Inicia o temporizador 
void timer_start() {
    pthread_t tid;
    if (pthread_create(&tid, NULL, temporizador_thread, NULL) != 0) {
        perror("Erro ao criar thread do timer");
        exit(1);
    }
}

// Retorna o tempo atual do sistema
int timer_get_time() {
    pthread_mutex_lock(&lock_timer);
    int t = tempo_atual;
    pthread_mutex_unlock(&lock_timer);
    return t;
}

// Verifica se houve estouro de tempo (slice)
bool timer_flag_slice() {
    pthread_mutex_lock(&lock_timer);
    bool status = flag_tempo;
    flag_tempo = false;
    pthread_mutex_unlock(&lock_timer);
    return status;
}
