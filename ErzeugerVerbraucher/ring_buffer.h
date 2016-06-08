//
// Created by s-gheldd on 5/30/16.
//

#include <zconf.h>
#include <stdlib.h>

#ifndef ERZEUGERVERBRAUCHER_RING_BUFFER_H
#define RING_BUFFER_BLOCK_SIZE 32
#define RING_BUFFER_SIZE 512
#define RING_BUFFER_SHM_KEY 0xabcde
#define RING_BUFFER_SHM_PERM 0666


struct buffer_entry {
    pid_t producer_pid;
    int number;
    char text[RING_BUFFER_BLOCK_SIZE];
};


int ring_buffer_init(void);

void ring_buffer_server_init(void);

int ring_buffer_write(struct buffer_entry *data, size_t size);

int ring_buffer_read(struct buffer_entry *buf);

int ring_buffer_dealloc(void);

int ring_buffer_server_dealloc(void);

//size_t ring_buffer_get_free_blocks_count(void);

#define ERZEUGERVERBRAUCHER_RING_BUFFER_H

#endif //ERZEUGERVERBRAUCHER_RING_BUFFER_H
