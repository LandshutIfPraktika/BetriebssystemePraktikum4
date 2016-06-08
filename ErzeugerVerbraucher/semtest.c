//
// Created by s-gheldd on 6/5/16.
//

#include <unistd.h>
#include <stdlib.h>
#include "updown.h"


int main() {
    if (fork()) {
        ud_sem_init();
        sleep(1);
        ud_sem_down();
        ud_sem_up();
    } else {
        ud_sem_init();
        ud_sem_down();
        sleep(1);
        ud_sem_up();

        sleep(5);
        ud_sem_down();
        ud_sem_dealloc();
    }
    return EXIT_SUCCESS;
}