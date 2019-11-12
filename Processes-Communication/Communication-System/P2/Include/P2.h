#ifndef P_H
#define P_H

#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include "Operations.h"

using namespace std;
void P2();


typedef struct thead_data
{
    int sem_p2;
    int sem_enc2;
    shm_data_1 *data2;
}thead_data;

#endif
