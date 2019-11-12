#include "P2.h"

void P2()
{
    int shm_message_P1,shm_message_P2;
    int sem_p1,sem_p2,sem_enc1,sem_enc2;
    int i;
    int err;
    shm_data_1 *data2;
    char message[MAX_CHAR];
    char *commun_message;
    char stdin_input[MAX_CHAR];


    sem_p2 = semget(SEM_P2_KEY,1,PERMS|IPC_CREAT);
    sem_enc2 = semget(SEM_ENC2_KEY,1,PERMS|IPC_CREAT);
    shm_message_P2 = shmget(SHM_P2_ENC2,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data2 = (shm_data_1 *)shmat(shm_message_P2,(void *)0,0);


    do
    {
        down(sem_p2);       //katebasma tou semaforous
            if(strcmp(data2->communication,"RESEND")==0)    //epanametadwsh
            {
                cout<<"->[P2]   Proccess P2 resending message..."<<endl;
                strcpy(data2->message,stdin_input);         //grapse to mhnhma
                strcpy(data2->communication,"SEND");        //enhmerwse ENC2
                up(sem_enc2);
            }
            if(strcmp(data2->communication,"TERM")==0)      //termatismos
            {
                cout<<"->[P2]   Proccess P2 terminates..."<<endl;
                break;
            }
            if(strcmp(data2->communication,"NULL")==0)      //perimene mhnhma apo xrhsth
            {
                cout<<"->[P2]   Sending from P2-->P1...\n";
                cout<<"->[P2]   Insert your message please...\n->[P2]   ";
                fgets(stdin_input,MAX_CHAR,stdin);
                stdin_input[strlen(stdin_input)-1]='\0';
                if(strcmp(stdin_input,"TERM")==0)
                {
                    cout<<"->[P2]   Proccess P2 terminates..."<<endl;
                    strcpy(data2->communication,"TERM");    //termatismos
                    up(sem_enc2);
                }
                else
                {
                    strcpy(data2->message,stdin_input);     //seile to mhnyma sthn ENC2
                    data2->length_message = strlen(stdin_input);
                    strcpy(data2->communication,"SEND");
                    cout<<"->[P2]   Sending message from P2-->ENC2...\n";
                    up(sem_enc2);
                }
            }
            if(strcmp(data2->communication,"RECEIVE")==0)   //lhpsh mhnhmatos
            {
                cout<<"->[P2]   P2 received message from P1\n";
                cout<<"->[P2]   \""<<data2->message<<"\""<<endl;
                strcpy(data2->communication,"NULL");
                up(sem_p2);
            }
    }
    while(strcmp(stdin_input,"TERM")!=0);

    exit(EXIT_SUCCESS);
}
