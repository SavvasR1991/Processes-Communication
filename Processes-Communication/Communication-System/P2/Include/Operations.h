#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>

#define MAX_CHAR 1024
#define MAX_CHAR_COMMAND 100

#define SHM_P1_ENC1 (key_t) 7889
#define SHM_P2_ENC2 (key_t) 7890
#define SHM_ENC1_CHAN (key_t) 7891
#define SHM_ENC2_CHAN (key_t) 7892

#define SEM_P1_KEY (key_t) 1216
#define SEM_P2_KEY (key_t) 1217
#define SEM_ENC1_KEY (key_t) 1218
#define SEM_ENC2_KEY (key_t) 1219
#define SEM_CHAN_KEY (key_t) 1220

#define PERMS 0666

union senum			//gia semaforoys
{
    int val;
    struct semid *buf;
    unsigned short *semarray;
};

typedef struct shm_data_1	//gia thn diamoirazomenh mnhmh
{
    char message[MAX_CHAR];
    char communication[MAX_CHAR_COMMAND];
    char check_sum[MAX_CHAR_COMMAND];
    int length_message;
}shm_data_1;


void down(int);

void up(int);


#endif
