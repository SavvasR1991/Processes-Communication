#ifndef ENC_H
#define ENC_H

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

void ENC1();

int de_hashing(char*,char*,int);
#endif
