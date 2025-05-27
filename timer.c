#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"
#include "CPU.h"

static int tempo_global = 0;
static bool flag_slice = false;
pthread_mutex_t mutex_timer = PTHREAD_MUTEX_INITIALIZER;

// Simular tempo em hardware
void *th_timer() {
    while (1) {
        usleep(10000);
        pthread_mutex_lock(&mutex_timer);
        tempo_global++;

        if (tempo_global % QUANTUM == 0) {
            flag_slice = true;
        }
        pthread_mutex_unlock(&mutex_timer);

        // printf("[timer] time=%d expired=%d\n", tempo_global, flag_slice);
    }
    return NULL;
}

// Start
void timer_start() {
    pthread_t tid;
    if (pthread_create(&tid, NULL, th_timer, NULL) != 0) {
        perror("th_timer");
        exit(1);
    }
}

// Get time
int timer_get_time() {
    pthread_mutex_lock(&mutex_timer);
    int t = tempo_global;
    pthread_mutex_unlock(&mutex_timer);
    return t;
}

// Atualiza flag
bool timer_flag_slice() {
    pthread_mutex_lock(&mutex_timer);
    bool v = flag_slice;
    flag_slice = false;
    pthread_mutex_unlock(&mutex_timer);
    return v;
}