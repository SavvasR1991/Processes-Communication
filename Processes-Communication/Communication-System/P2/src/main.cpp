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
#include "ENC2.h"
#include "P2.h"

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
    pid_t pid_array[2];
    senum arg;

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
        cout<<"->Proccess P2   created ,id ["<<getpid()<<"]..."<<endl;
        P2();
    }

    pid = fork();
    pid_array[1]=pid;
    if(pid==0)
    {
        cout<<"->Proccess ENC2 created ,id ["<<getpid()<<"]..."<<endl;
        ENC2();
    }
    while ((wpid = wait(&status)) > 0);


    cout<<"//////////////////////////////////PROGRAMM ENDS////////////////////////////////////\n";
    cout<<"|*********************************************************************************|\n\n";
    return 0;
}
