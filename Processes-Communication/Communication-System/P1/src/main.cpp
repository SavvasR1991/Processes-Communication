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
#include "CHAN.h"
#include "ENC.h"
#include "P.h"

using namespace std;

int main (int argc, char** argv){
    int i;
    int shm_message,shm_P1_ENC1,shm_P2_ENC2,shm_ENC1_CHAN,shm_ENC2_CHAN;
    int sem_p1_enc1,sem_p2_enc2,sem_message;
    int sem_enc1_p1,sem_enc2_p2;
    int sem_enc1_chan,sem_enc2_chan;
    int sem_chan_enc1,sem_chan_enc2;
    int status;
    char *data;
    double possibility;
    shm_data_1 *data_enc1_chan;
    shm_data_1 *data_enc2_chan;
    shm_data_1 *data_p1_enc1;
    shm_data_1 *data_p2_enc2;
    pid_t pid,wpid;
    pid_t pid_array[3];
    senum arg;
                        ///////////////////
                        //SHARED MEMORIES//
////////////////////////////////////////////////////////////////////////////////////
    shm_ENC1_CHAN = shmget(SHM_ENC1_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_enc1_chan = (shm_data_1 *)shmat(shm_ENC1_CHAN,(void *)0,0);
    memcpy(&(data_enc1_chan->communication),"NULL",sizeof("NULL"));

    shm_ENC2_CHAN = shmget(SHM_ENC2_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_enc2_chan = (shm_data_1 *)shmat(shm_ENC2_CHAN,(void *)0,0);
    memcpy(&(data_enc2_chan->communication),"NULL",sizeof("NULL"));

    shm_P1_ENC1 = shmget(SHM_P1_ENC1,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_p1_enc1 = (shm_data_1 *)shmat(shm_P1_ENC1,(void *)0,0);
    memcpy(&(data_p1_enc1->communication),"NULL",sizeof("NULL"));

    shm_P2_ENC2 = shmget(SHM_P2_ENC2,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_p2_enc2 = (shm_data_1 *)shmat(shm_P2_ENC2,(void *)0,0);
    memcpy(&(data_p2_enc2->communication),"NULL",sizeof("NULL"));
/////////////////////////////////////////////////////////////////////////////////////

                        //////////////
                       //SEMAPHORES//
///////////////////////////////////////////////////////////////////////
    sem_message = semget(SEM_CHAN_KEY,1,PERMS|IPC_CREAT);             //
    arg.val=0;                                                       //
    semctl(sem_message,0,SETVAL,arg);                                //
                                                                     //
    sem_p1_enc1 = semget(SEM_P1_KEY,1,PERMS|IPC_CREAT);         //
    arg.val=1;                                                       //
    semctl(sem_p1_enc1,0,SETVAL,arg);                                //
                                                                     //
    sem_p2_enc2 = semget(SEM_P2_KEY,1,PERMS|IPC_CREAT);         //
    arg.val=0;                                                       //
    semctl(sem_p2_enc2,0,SETVAL,arg);                                //
                                                                     //
    sem_enc1_chan = semget(SEM_ENC1_KEY,1,PERMS|IPC_CREAT);     //
    arg.val=0;                                                       //
    semctl(sem_enc1_chan,0,SETVAL,arg);                              //
                                                                     //
    sem_enc2_chan = semget(SEM_ENC2_KEY,1,PERMS|IPC_CREAT);     //
    arg.val=0;                                                       //
    semctl(sem_enc2_chan,0,SETVAL,arg);                              //
///////////////////////////////////////////////////////////////////////
    if(argc>2)
    {
        cout<<"->You have insert to many arguments..."<<endl;
        return 0;
    }
    if(argc!=2)
    {
        cout<<"->You have not insert possibility..."<<endl;
        return 0;
    }
    possibility=atof(argv[1]);
    while(possibility>100||possibility<0)
    {
        cout<<"->Possibilty can not be negative or bigger than 100..\n";
        cout<<"->Please insert possibility...\n->";
        cin>>possibility;
    }
    if(possibility<1&&possibility>0)
    {
        possibility=possibility*100;
    }
    cout<<"\n\n|*********************************************************************************|";
    cout<<"\n//////////////////////////////////PROGRAMM STARTS//////////////////////////////////"<<endl;
    cout<<"->\n->/*********************STATUS***********************/"<<endl;
    cout<<"->/   Possibility for message mistake :"<<possibility<<"%           /"<<endl;
    cout<<"->/   Total procceses                 :5 proccesses  /"<<endl;
    cout<<"->/   Total semaphores                :5 semaphores  /"<<endl;
    cout<<"->/   Total shared memories           :4 sh.mem.     /"<<endl;
    cout<<"->/   To exit programm insert         :\"TERM\"        /"<<endl;
    cout<<"->/**************************************************/\n->"<<endl;
    pid = fork();
    pid_array[0]=pid;
    if(pid==0)
    {
        cout<<"->Proccess P1   created ,id ["<<getpid()<<"]..."<<endl;
        P();
    }

    pid = fork();
    pid_array[1]=pid;
    if(pid==0)
    {
        cout<<"->Proccess ENC1 created ,id ["<<getpid()<<"]..."<<endl;
        ENC1();
    }

    pid = fork();
    pid_array[2]=pid;
    if(pid==0)
    {
        cout<<"->Proccess CHAN created ,id ["<<getpid()<<"]..."<<endl;
        CHAN(possibility);
    }

    while ((wpid = wait(&status)) > 0);
    cout<<"->Free semaphores and shared memories..."<<endl;

    shmctl(shm_message,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shm_ENC1_CHAN,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shm_ENC2_CHAN,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shm_P1_ENC1,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shm_P2_ENC2,IPC_RMID,(struct shmid_ds*)0);

    semctl(sem_message,1,IPC_RMID,0);
    semctl(sem_p1_enc1,1,IPC_RMID,0);
    semctl(sem_p2_enc2,1,IPC_RMID,0);
    semctl(sem_enc1_chan,1,IPC_RMID,0);
    semctl(sem_enc2_chan,1,IPC_RMID,0);
    cout<<"//////////////////////////////////PROGRAMM ENDS////////////////////////////////////\n";
    cout<<"|*********************************************************************************|\n\n";
    return 0;
}
