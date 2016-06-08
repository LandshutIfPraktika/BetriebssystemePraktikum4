//
// Created by s-gheldd on 5/30/16.
//

#include "updown.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

static struct sembuf semaphore;
static int sem_id;

static int sem_op(const int op, const char *type);


int ud_sem_init(void) {
    sem_id = semget(UD_SEM_KEY, 0, IPC_PRIVATE);
    if (sem_id < 0) {
        umask(0);
        sem_id = semget(UD_SEM_KEY, 1, IPC_CREAT | IPC_EXCL | UD_SEM_PERM);
        if (sem_id < 0) {
            perror("Semaphore creation failed");
            exit(EXIT_FAILURE);
        }
        if (semctl(sem_id, 0, SETVAL, 1) == -1) {
            perror("Semaphore initialization failed");
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

int ud_sem_dealloc(void) {
    if (semctl(sem_id, 0, IPC_RMID) < 0) {
        perror("Could not remove semaphore");
        exit(EXIT_FAILURE);
    }
    return 1;
}

static int sem_op(const int op, const char *type) {
    semaphore.sem_num = 0;
    semaphore.sem_op = op;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(sem_id, &semaphore, 1) < 0) {
        char buf[256];
        sprintf(buf, "Semaphore operation \"%s\" failed", type);
        perror(buf);
        return -1;
    }
    return 1;
}

int ud_sem_up() {
    return sem_op(1, "UP");
}


int ud_sem_down() {
    return sem_op(-1, "DOWN");
}