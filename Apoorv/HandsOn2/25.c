/* 
MT2020013 Apoorv Panse
25. Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcvs
*/

#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>

int main()
{
    int messageId;
    key_t key;
    key=ftok("./25.c",100);
    
    messageId=msgget(key,IPC_CREAT|0777);   
    if(messageId < 0)
    {	
        printf("%s\n",strerror(errno));
        exit(0);
        
    }
    
    struct msqid_ds msg;
    struct ipc_perm msg_perm;
    msgctl(messageId, IPC_STAT, &msg);
    msg_perm=msg.msg_perm;

    printf("Acess permission : %d\n",msg_perm.mode & 0777);
    printf("UID : %d\n",msg_perm.uid);
    printf("GID : %d\n",msg_perm.gid);
    printf("Time of last message sent : %s",ctime(&msg.msg_stime));
    printf("Time of last message recieved : %s",ctime(&msg.msg_rtime));
    printf("Time of last change in queue : %s",ctime(&msg.msg_ctime));
    printf("Size of the queue : %ld\n",msg.__msg_cbytes);
    printf("Number of messages in queue : %ld\n",msg.msg_qnum);
    printf("Maximum number of bytes allowed : %ld\n",msg.msg_qbytes);
    printf("PID of message sent : %d\n",msg.msg_lspid);
    printf("PID of message recieved : %d\n",msg.msg_lrpid);
    
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Acess permission : 511
UID : 1000
GID : 1000
Time of last message sent : Thu Jan  1 05:30:00 1970
Time of last message recieved : Thu Jan  1 05:30:00 1970
Time of last change in queue : Tue Oct 12 16:12:27 2021
Size of the queue : 0
Number of messages in queue : 0
Maximum number of bytes allowed : 16384
PID of message sent : 0
PID of message recieved : 0


*/