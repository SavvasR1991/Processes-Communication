#include "Operations.h"

struct sembuf inc[1] = {0,1,0};

struct sembuf dec[1] = {0,-1,0};

void up(int semid)
{
    semop(semid,&inc[0],1);
}

void down(int semid)
{
    semop(semid,&dec[0],1);
}
