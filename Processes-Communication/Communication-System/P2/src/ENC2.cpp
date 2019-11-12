#include "ENC2.h"
#include "MD5.h"
void ENC2()
{
    int shm_p1_enc1,shm_enc1_chan;
    int shm_p2_enc2,shm_enc2_chan;
    int sem_p1,sem_enc1;
    int sem_chan;
    int sem_p2,sem_enc2;
    int i;
    int len;
    int error_check;
    int hash_result;
    shm_data_1 *data_p1_enc1,*data_enc1_chan;
    shm_data_1 *data_p2_enc2,*data_enc2_chan;
    char message[MAX_CHAR];
    char commun_message[MAX_CHAR_COMMAND];
    char check_sum[MAX_CHAR];
    char hash_res[MAX_CHAR_COMMAND];
    char temp[MAX_CHAR];
    string tem;
    sem_chan = semget(SEM_CHAN_KEY,1,PERMS|IPC_CREAT);

    sem_p2 = semget(SEM_P2_KEY,1,PERMS|IPC_CREAT);
    sem_enc2 = semget(SEM_ENC2_KEY,1,PERMS|IPC_CREAT);

    shm_p2_enc2 = shmget(SHM_P2_ENC2,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_p2_enc2 = (shm_data_1 *)shmat(shm_p2_enc2,(void *)0,0);
    shm_enc2_chan = shmget(SHM_ENC2_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
    data_enc2_chan = (shm_data_1 *)shmat(shm_enc2_chan,(void *)0,0);
    do
    {
        error_check=0;
        down(sem_enc2);
            if(strcmp(data_enc2_chan->communication,"TERM")==0)//termatismos
            {
                cout<<"->[ENC2] Proccess ENC2 terminates..."<<endl;
                strcpy((data_p2_enc2->communication),"TERM");
                up(sem_p2);
                break;
            }
            if(strcmp(data_p2_enc2->communication,"TERM")==0)//termatismos
            {
                cout<<"->[ENC2] Proccess ENC2 terminates..."<<endl;
                strcpy((data_enc2_chan->communication),"TERM");
                up(sem_chan);
                break;
            }
            else if(strcmp(data_enc2_chan->communication,"RESEND")==0)
            {
                cout<<"->[ENC2] Request ENC2->P2"<<endl;//epanametadosh
                strcpy((data_p2_enc2->communication),"RESEND");
                strcpy(data_enc2_chan->communication,"NULL");
                up(sem_p2);
            }
            else if(strcmp(data_p2_enc2->communication,"SEND")==0)
            {
                data_enc2_chan->length_message=data_p2_enc2->length_message;
                tem=md5(data_p2_enc2->message);//aposto;h
                len = tem.size();//mhnhmatos
                for(i=0;i<len;i++)
                {
                    check_sum[i] = tem[i];
                }
                check_sum[i] = '\0';

                strcpy(hash_res,data_p2_enc2->message);
                strcat(hash_res,check_sum);
                cout<<"->[ENC2] Encapsulation :\""<<data_p2_enc2->message<<"\" \n->[ENC2]               :\""<<check_sum<<"\n";
                strcpy((data_enc2_chan->check_sum),check_sum);//enthulakwsh
                strcpy((data_enc2_chan->message),hash_res);

                strcpy((data_enc2_chan->communication),"ENC2");
                strcpy((data_p2_enc2->communication),"NULL");
                cout<<"->[ENC2] Sending from ENC2-->CHAN...\n";
                up(sem_chan);
            }
            else if(strcmp(data_enc2_chan->communication,"RECEIVE")==0)
            {
                cout<<"->[ENC2] Received message\n->[ENC2] \""<<data_enc2_chan->message<<"\"";
                cout<<"  \n->[ENC2] check_sum   ["<<data_enc2_chan->check_sum<<"]"<<endl;
                error_check=de_hashing(data_enc2_chan->message,data_enc2_chan->check_sum,data_enc2_chan->length_message);
                strcpy((data_enc2_chan->communication),"NULL");//lhspsh mhnhmatos
                if(error_check==1)
                {                               //lathos sto check sum
                    cout<<"->[ENC2] Checksum has changed,invalid checksum..."<<endl;
                    cout<<"->[ENC2] Message not send..."<<endl;
                    strcpy((data_p2_enc2->communication),"NULL");
                    up(sem_p2);
                }
                else if(error_check==2)
                {                               //lathos sto mhnhma
                    cout<<"->[ENC2] Message has changed..."<<endl;
                    cout<<"->[ENC2] Request from P1 to resend the message..."<<endl;
                    strcpy((data_enc2_chan->communication),"RESEND");
                    up(sem_chan);
                }
                else
                {                               //apostolh mhnhmatos
                    cout<<"->[ENC2] Sending from ENC2-->P2...\n";
                    strcpy((data_p2_enc2->communication),"RECEIVE");
                    for(i=0;i<data_enc2_chan->length_message;i++)
                    {
                        message[i]=data_enc2_chan->message[i];
                    }
                    message[i]='\0';
                    strcpy(data_p2_enc2->message,message);
                    up(sem_p2);
                }
            }
            else
            {
                cout<<"ERROR IN ENC2 ...sustem corrupted\n";
                exit(EXIT_FAILURE);
            }
    }
    while(1);
    shmdt(data_enc2_chan);
    shmdt(data_p2_enc2);
    exit(EXIT_SUCCESS);
}


//sunarthsh gia thn apotulakwsh tou mhnumatos
int de_hashing(char *message_checksum,char* checksum,int length)
{
    char message[MAX_CHAR];
    char check_sum_temp[MAX_CHAR];
    int i;
    int j=0,len;
    string tem;

    for(i=0;i<strlen(message_checksum);i++) //diaxwrismos mhnumatos
    {
        if(i<length)
        {
            message[i]=message_checksum[i];
        }
        else                                //diaxwrismos check sum
        {
           check_sum_temp[j]=message_checksum[i];
           j++;
        }
    }
    message[length]='\0';
    check_sum_temp[j]='\0';
    if(strcmp(check_sum_temp,checksum)!=0) //elegxos gia check sum
    {
        return 1;
    }
    tem = md5(message); //elegxos gia orthotita mhnumatos
    len = tem.size();
    for(i=0;i<len;i++)
    {
        check_sum_temp[i] = tem[i];
    }
    check_sum_temp[i] = '\0';//elegxos mhnhmatos 
    if(strcmp(checksum,check_sum_temp)!=0)
    {
        return 2;
    }
    return 0;
}

