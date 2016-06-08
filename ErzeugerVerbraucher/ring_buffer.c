//
// Created by s-gheldd on 5/30/16.
//
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ring_buffer.h"
#include "updown.h"

static size_t ring_buffer_get_free_blocks_count(void);

#define ring_buffer_block_number 12

struct ring_buffer {
    struct buffer_entry entries[ring_buffer_block_number];
    int last_read;
    int last_written;
    int rnd;
};

static int shm_id;
static struct ring_buffer *ring_buffer;

static size_t ring_buffer_get_free_blocks_count(void) {
    size_t free;
    if (ring_buffer->rnd) {
        free = (size_t) ring_buffer->last_read - ring_buffer->last_written;
    } else {
        free = (size_t) (ring_buffer_block_number - ring_buffer->last_written + ring_buffer->last_read);
    }
    //printf("written %d, read %d, round %d => free %d \n", ring_buffer->last_written, ring_buffer->last_read,
    //       ring_buffer->rnd, free);
    return free;
}

int ring_buffer_init(void) {
    shm_id = shmget(RING_BUFFER_SHM_KEY, 0, IPC_PRIVATE);
    if (shm_id == -1) {
        shm_id = shmget(RING_BUFFER_SHM_KEY, sizeof(struct ring_buffer), IPC_CREAT | IPC_EXCL | RING_BUFFER_SHM_PERM);
        if (shm_id == -1) {
            perror("Could not create shared memory");
            exit(EXIT_FAILURE);
        }
    }
    ring_buffer = (struct ring_buffer *) shmat(shm_id, NULL, 0);
    if (ring_buffer == (void *) -1) {
        perror("Could not attach shared memory");
        exit(EXIT_FAILURE);
    }
    return ud_sem_init();
}

void ring_buffer_server_init(void) {
    ring_buffer->last_read = ring_buffer_block_number - 1;
    ring_buffer->last_written = ring_buffer_block_number - 1;
    ring_buffer->rnd = 0;
}

int ring_buffer_dealloc(void) {
    ud_sem_down();
    if (shmdt((void *) ring_buffer) < 0) {
        perror("could not detach shared memory");
        exit(EXIT_FAILURE);
    }
    ud_sem_up();
    return 1;
}

int ring_buffer_server_dealloc(void) {
    ud_sem_down();
    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
        perror("could not delete shared memory");
        exit(EXIT_FAILURE);
    }
    ud_sem_dealloc();
    return 1;
}

int ring_buffer_write(struct buffer_entry *data, size_t size) {
    int pos = 0;
    while (pos < size) {
        int free;
        ud_sem_down();
        while (ring_buffer_get_free_blocks_count() > 0 && pos < size) {
            //free = ring_buffer_get_free_blocks_count();
            ring_buffer->last_written = ++(ring_buffer->last_written) % ring_buffer_block_number;
            if (!ring_buffer->last_written) {
                //printf("round\n");
                ring_buffer->rnd = 1;
            }
            //printf(" %d free wrtitten to %d \n", free, ring_buffer->last_written);
            memcpy(&(ring_buffer->entries[ring_buffer->last_written]), &(data[pos]), sizeof(struct buffer_entry));
            //ring_buffer->entries[ring_buffer->last_written] = data[pos];
            pos++;
        }
        ud_sem_up();
    }
    return pos;
}

int ring_buffer_read(struct buffer_entry *buf) {
    int read = 0;
    while (!read) {
        ud_sem_down();
        if (ring_buffer->last_written != ring_buffer->last_read
            || (ring_buffer->last_written == ring_buffer->last_read && ring_buffer->rnd)) {
            ring_buffer->last_read = ++(ring_buffer->last_read) % ring_buffer_block_number;
            if (!ring_buffer->last_read) {
                ring_buffer->rnd = 0;
            }
            memcpy(buf, &(ring_buffer->entries[ring_buffer->last_read]), sizeof(struct buffer_entry));

            //*buf = ring_buffer->entries[ring_buffer->last_read];
            read = 1;
        }
        ud_sem_up();
    }
    return read;
}