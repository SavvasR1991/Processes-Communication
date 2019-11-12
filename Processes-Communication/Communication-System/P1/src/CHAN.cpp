#include "CHAN.h"

void CHAN(double possibility)
{
    char message[MAX_CHAR];
    int total_letters;
    int changed_letters;
    int sem_enc1;
    int sem_enc2;
    int sem_chan;
    int shm_enc1_chan;
    int shm_enc2_chan;
    int i;
    int rand_result;
    char rand_char;
    shm_data_1 *data_enc1_chan;
    shm_data_1 *data_enc2_chan;
    srand (time(NULL));

    sem_enc1 = semget(SEM_ENC1_KEY,1,PERMS|IPC_CREAT);
    sem_enc2 = semget(SEM_ENC2_KEY,1,PERMS|IPC_CREAT);
    sem_chan = semget(SEM_CHAN_KEY,1,PERMS|IPC_CREAT);

    shm_enc1_chan = shmget(SHM_ENC1_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_enc1_chan = (shm_data_1 *)shmat(shm_enc1_chan,(void *)0,0);
    shm_enc2_chan = shmget(SHM_ENC2_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_enc2_chan = (shm_data_1 *)shmat(shm_enc2_chan,(void *)0,0);
    do
    {
        down(sem_chan);
            if(strcmp(data_enc1_chan->communication,"TERM")==0)//termatismos
            {
                cout<<"->[CHAN] Proccess CHAN terminates..."<<endl;//apo ENC!
                strcpy(data_enc2_chan->communication,"TERM");
                up(sem_enc2);
                break;
            }
            if(strcmp(data_enc2_chan->communication,"TERM")==0)//termatismos
            {
                cout<<"->[CHAN] Proccess CHAN terminates..."<<endl;//apo ENC2
                strcpy(data_enc1_chan->communication,"TERM");
                up(sem_enc1);
                break;
            }
            if(strcmp(data_enc2_chan->communication,"RESEND")==0&&strcmp(data_enc1_chan->communication,"NULL")==0)
            {
                cout<<"->[CHAN] Request CHAN->ENC1"<<endl;//epanametadosh
                strcpy(data_enc1_chan->communication,"RESEND");//apo ENC2
                strcpy(data_enc2_chan->communication,"NULL");  
                up(sem_enc1);
    
            }
            else if(strcmp(data_enc1_chan->communication,"RESEND")==0&&strcmp(data_enc2_chan->communication,"NULL")==0)
            {
                cout<<"->[CHAN] Request CHAN->ENC2"<<endl;//epanametadosh
                strcpy(data_enc2_chan->communication,"RESEND");//apo ENC1
                strcpy(data_enc1_chan->communication,"NULL");       
                up(sem_enc2);
            }
            else if(strcmp(data_enc1_chan->communication,"ENC1")==0&&strcmp(data_enc2_chan->communication,"NULL")==0)
            {
                strcpy(data_enc1_chan->communication,"NULL");//allagh mhnhmatos
                cout<<"->[CHAN] Channel might change the message \n->[CHAN] \""<<data_enc1_chan->message<<"\""<<endl;
                total_letters=strlen(data_enc1_chan->message);//apo ENC1
                changed_letters=possibility*total_letters/100;
                for(i=0;i<changed_letters;i++)                  //allagh
                {
                    rand_result = rand()%total_letters;
                    rand_char = (rand()%26)+97;
                    data_enc1_chan->message[rand_result]=rand_char;
                }
                cout<<"->[CHAN] Sending from CHAN->ENC2..."<<endl;//apostolh 
                strcpy(data_enc2_chan->message,data_enc1_chan->message);//allagmenou mhnhmatos
                strcpy(data_enc2_chan->check_sum,data_enc1_chan->check_sum);
                data_enc2_chan->length_message=data_enc1_chan->length_message;
                strcpy((data_enc2_chan->communication),"RECEIVE");       
                up(sem_enc2);
            }
            else if(strcmp(data_enc2_chan->communication,"ENC2")==0&&strcmp(data_enc1_chan->communication,"NULL")==0)
            {
                strcpy((data_enc2_chan->communication),"NULL");//allagh mhnhmatos
                cout<<"->[CHAN] Channel might change the message \n->[CHAN] \""<<data_enc2_chan->message<<"\""<<endl;
                total_letters=strlen(data_enc2_chan->message);//apo ENC2
                rand_result = rand()%100;
                if(rand_result<possibility)//an mirotero apo pithanothta
                {                           //allagh
                    changed_letters=possibility*total_letters/100;
                    for(i=0;i<changed_letters;i++)      //allagh 
                    {
                        rand_result = rand()%total_letters;
                        rand_char = (rand()%26)+97;
                        data_enc2_chan->message[rand_result]=rand_char;
                    }
                }
                cout<<"->[CHAN] Sending from CHAN->ENC1..."<<endl;//apostolh 
                strcpy(data_enc1_chan->message,data_enc2_chan->message);
                strcpy(data_enc1_chan->check_sum,data_enc2_chan->check_sum);
                data_enc1_chan->length_message=data_enc2_chan->length_message;
                strcpy((data_enc1_chan->communication),"RECEIVE");       
                up(sem_enc1);
            }
            else
            {
                cout<<"->ERROR IN CHAN ,system corrupted.."<<endl;
                exit(EXIT_FAILURE);
            }
    }
    while(strcmp(data_enc1_chan->communication,"TERM")!=0);
    shmdt(data_enc1_chan);
    exit(EXIT_SUCCESS);
}
