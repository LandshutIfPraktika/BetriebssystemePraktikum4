//
// Created by s-gheldd on 5/30/16.
//

#ifndef ERZEUGERVERBRAUCHER_UPDOWN_H
#define UD_SEM_KEY 0xabcd
#define UD_SEM_PERM 0666

int ud_sem_dealloc(void);

int ud_sem_init(void);

int ud_sem_up(void);

int ud_sem_down(void);

#define ERZEUGERVERBRAUCHER_UPDOWN_H

#endif //ERZEUGERVERBRAUCHER_UPDOWN_H
