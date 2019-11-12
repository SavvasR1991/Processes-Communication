#include "P.h"

void P()
{
    int shm_message_P1,shm_message_P2;
    int sem_p1,sem_p2,sem_enc1,sem_enc2;
    int i;
    shm_data_1 *data,*data2;
    char message[MAX_CHAR];
    char *commun_message;
    char stdin_input[MAX_CHAR];
    sem_p1 = semget(SEM_P1_KEY,1,PERMS|IPC_CREAT);
    sem_enc1 =semget(SEM_ENC1_KEY,1,PERMS|IPC_CREAT);
    shm_message_P1 = shmget(SHM_P1_ENC1,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data = (shm_data_1 *)shmat(shm_message_P1,(void *)0,0);
    sleep(1);
    do
    {
        down(sem_p1);
            if(strcmp(data->communication,"RESEND")==0) //anametadosh
            {
                cout<<"->[P1]   Proccess P1 resending message. \""<<stdin_input<<"\""<<endl;
                strcpy(data->message,stdin_input);
                strcpy(data->communication,"SEND");
                up(sem_enc1);
            }
            if(strcmp(data->communication,"TERM")==0)   //termatismos
            {
                cout<<"->[P1]   Proccess P1 terminates..."<<endl;
                break;
            }
            if(strcmp(data->communication,"NULL")==0)   //eisagwgh munhmatos
            {
                cout<<"->[P1]   Sending from P1-->P2...\n";
                cout<<"->[P1]   Insert your message please...\n->[P1]   ";
                fgets(stdin_input,MAX_CHAR,stdin);
                stdin_input[strlen(stdin_input)-1]='\0';
                if(strcmp(stdin_input,"TERM")==0)   //periptwsh termatismou 
                {
                    cout<<"->[P1]   Proccess P1 terminates..."<<endl;

                    strcpy(data->communication,"TERM");
                }
                else
                {
                    strcpy(data->message,stdin_input);  //eisagwgh mhnhmatos
                    data->length_message = strlen(stdin_input);
                    strcpy(data->communication,"SEND");
                    cout<<"->[P1]   Sending message from P1-->ENC1 ,key ["<<SEM_ENC1_KEY<<"]...\n";
                }
                up(sem_enc1);
            }
            if(strcmp(data->communication,"RECEIVE")==0)    //lhpsh mhnhmatos
            {
                cout<<"->[P1]   P1 received message from P2\n";
                cout<<"->[P1]   \""<<data->message<<"\""<<endl;
                strcpy(data->communication,"NULL");
                up(sem_p1);
            }
        }
        while(strcmp(stdin_input,"TERM")!=0);
        shmdt(data);
        exit(EXIT_SUCCESS);
}
