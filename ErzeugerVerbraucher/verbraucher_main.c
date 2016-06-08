//
// Created by s-gheldd on 6/8/16.
//
#include <stdio.h>
#include <signal.h>
#include "ring_buffer.h"


int main(void) {
    pid_t erzeuger_pid;
    int number = 1;
    struct buffer_entry buffer;
    ring_buffer_init();
    while (number != -1) {
        ring_buffer_read(&buffer);
        number = buffer.number;
        erzeuger_pid = buffer.producer_pid;
        printf("%s", buffer.text);
    }
    kill(erzeuger_pid, SIGUSR1);
    exit(EXIT_SUCCESS);
}