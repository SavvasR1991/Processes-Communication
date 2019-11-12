#include "Operations.h"

struct sembuf inc[1] = {0,1,0}; //dhlwseis gia tous semaforoys

struct sembuf dec[1] = {0,-1,0};

void up(int semid)              // up
{
    semop(semid,&inc[0],1);
}

void down(int semid)            //down
{
    semop(semid,&dec[0],1);
}
