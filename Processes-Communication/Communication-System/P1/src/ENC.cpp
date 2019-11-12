#include "ENC.h"
#include "MD5.h"
void ENC1()
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

        sem_p1 = semget(SEM_P1_KEY,1,PERMS|IPC_CREAT);
        sem_enc1 = semget(SEM_ENC1_KEY,1,PERMS|IPC_CREAT);

        shm_p1_enc1 = shmget(SHM_P1_ENC1,sizeof(shm_data_1),PERMS|IPC_CREAT);
        data_p1_enc1 = (shm_data_1 *)shmat(shm_p1_enc1,(void *)0,0);
        shm_enc1_chan = shmget(SHM_ENC1_CHAN,sizeof(shm_data_1),PERMS|IPC_CREAT);
        data_enc1_chan = (shm_data_1 *)shmat(shm_enc1_chan,(void *)0,0);

        do
        {
            error_check=0;
            down(sem_enc1);
                if(strcmp(data_p1_enc1->communication,"TERM")==0)   //termatismos
                {
                    cout<<"->[ENC1] Proccess ENC1 terminates..."<<endl;
                    strcpy(data_enc1_chan->communication,"TERM");
                    up(sem_chan);
                    break;

                }
                else if(strcmp(data_enc1_chan->communication,"TERM")==0)//termatismos
                {
                    cout<<"->[ENC1] Proccess ENC1 terminates..."<<endl;
                    strcpy(data_p1_enc1->communication,"TERM");
                    up(sem_p1);
                    break;

                }
                else if(strcmp(data_enc1_chan->communication,"RESEND")==0)
                {
                    cout<<"->[ENC1] Request ENC1->P1"<<endl;//epanametadosh
                    strcpy((data_p1_enc1->communication),"RESEND");//mhnhmatos
                    strcpy((data_enc1_chan->communication),"NULL");
                    up(sem_p1);
                }
                else if(strcmp(data_p1_enc1->communication,"SEND")==0)//apostolh
                {
                    data_enc1_chan->length_message=data_p1_enc1->length_message;
                    tem=md5(data_p1_enc1->message);
                    len = tem.size();
                    for(i=0;i<len;i++)              //enthulakwsh mhnhmatos
                    {
                        check_sum[i] = tem[i];
                    }
                    check_sum[i] = '\0';
                    strcpy(hash_res,data_p1_enc1->message);
                    strcat(hash_res,check_sum);
                    cout<<"->[ENC1] Encapsulation : *\""<<data_p1_enc1->message<<"\" \n->[ENC1]               : *\""<<check_sum<<"\n";
                    strcpy((data_enc1_chan->check_sum),check_sum);//apostolh
                    strcpy((data_enc1_chan->message),hash_res);//sto kanali
                    strcpy((data_enc1_chan->communication),"ENC1");
                    cout<<"->[ENC1] Sending from ENC1-->CHAN ,key ["<<SEM_CHAN_KEY<<"]...\n";
                    strcpy((data_p1_enc1->communication),"NULL");
                    up(sem_chan);
                }
                else if(strcmp(data_enc1_chan->communication,"RECEIVE")==0)
                {
                    cout<<"->[ENC1] Received message\n->[ENC1]             *\""<<data_enc1_chan->message<<"\"\n";
                    cout<<"->[ENC1] check_sum   *["<<data_enc1_chan->check_sum<<"]"<<endl;
                    error_check=de_hashing(data_enc1_chan->message,data_enc1_chan->check_sum,data_enc1_chan->length_message);
                    strcpy((data_enc1_chan->communication),"NULL");//lhpsh mhnhmatos 
                    if(error_check==1)//lathos sto check sum
                    {
                        cout<<"->[ENC1] Checksum has changed,invalid checksum..."<<endl;
                        cout<<"->[ENC1] Message not send..."<<endl;
                        strcpy((data_p1_enc1->communication),"NULL");
                        up(sem_p1);
                    }
                    else if(error_check==2)     //lathos sto mhnhma 
                    {
                        cout<<"->[ENC1] Message has changed..."<<endl;
                        cout<<"->[ENC1] Request from P2 to resend the message..."<<endl;
                        strcpy((data_enc1_chan->communication),"RESEND");
                        up(sem_chan);
                    }
                    else                    //apostolh
                    {
                        cout<<"->[ENC1] Sending from ENC1-->P1...\n";
                        strcpy((data_p1_enc1->communication),"RECEIVE");
                        len=strlen(data_enc1_chan->check_sum);
                        for(i=0;i<data_enc1_chan->length_message;i++)
                        {
                            message[i]=data_enc1_chan->message[i];
                        }
                        message[i]='\0';
                        strcpy(data_p1_enc1->message,message);
                        up(sem_p1);
                    }
                }
                else
                {
                    cout<<"ERROR IN ENC1,system corrupted....\n";
                    exit(EXIT_FAILURE);

                }
        }
        while(1);
        shmdt(data_p1_enc1);
        shmdt(data_enc1_chan);
        exit(EXIT_SUCCESS);

}



int de_hashing(char *message_checksum,char* checksum,int length)
{       //sunarthsh gia apothilakwsh
    char message[MAX_CHAR];
    char check_sum_temp[MAX_CHAR];
    int i;
    int j=0,len;
    string tem;

    for(i=0;i<strlen(message_checksum);i++)
    {
        if(i<length)
        {
            message[i]=message_checksum[i];//ksexwrise to mhnhma
        }
        else
        {
           check_sum_temp[j]=message_checksum[i];//ksexwrise to check sum
           j++;
        }
    }
    message[length]='\0';
    check_sum_temp[j]='\0';
    if(strcmp(check_sum_temp,checksum)!=0)//elegxos check sum
    {
        return 1;
    }
    tem = md5(message);     //elengos mhnhmatos
    len = tem.size();
    for(i=0;i<len;i++)
    {
        check_sum_temp[i] = tem[i];// elekse to mhnhma
    }
    check_sum_temp[i] = '\0';                    
    if(strcmp(checksum,check_sum_temp)!=0)//an oxi steile lathos
    {
        return 2;
    }
    return 0;
}

