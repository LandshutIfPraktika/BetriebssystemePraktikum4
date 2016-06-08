//
// Created by s-gheldd on 6/5/16.
//

#include <zconf.h>
#include <stdio.h>
#include "ring_buffer.h"

int main() {
    if (!fork()) {
        struct buffer_entry entry[40];
        int i;
        printf("parent %d\n", getpid());
        ring_buffer_init();
        ring_buffer_server_init();

        for (i = 0; i < 40; i++) {
            entry[i].number = i;
            entry[i].producer_pid = getpid();
            sprintf(entry[i].text, "hallo %c", ('A' + i));
        }

        entry[39].number = -1;


        ring_buffer_write(entry, 40);
        sleep(2);
        ring_buffer_dealloc();
        ring_buffer_server_dealloc();

    } else {
        int number = 1;
        struct buffer_entry entry;
        printf("child %d\n", getpid());
        ring_buffer_init();
        while (number != -1) {
            ring_buffer_read(&entry);
            printf("Pozess: %d, number: %d, data: %s\n", entry.producer_pid, entry.number, entry.text);
            number = entry.number;
        }
        ring_buffer_dealloc();

    }

/*    struct buffer_entry entry;
    ring_buffer_init();
    ring_buffer_server_init();
    ring_buffer_get_free_blocks_count();
    for (int i = 0; i < 12; i++) {
        ring_buffer_write(&entry, 1);
    }
    ring_buffer_read(&entry);
    ring_buffer_write(&entry, 1);
    ring_buffer_get_free_blocks_count();*/
}