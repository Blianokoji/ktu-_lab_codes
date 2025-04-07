#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT 5000
#define sa struct sockaddr
#define TIMEOUT 3


int timecheck(int a){
    struct timeval time;
    gettimeofday(&time,NULL);
    return time.tv_sec - a;
}

typedef struct{
    int data;
    struct timeval sent_time;
    int ack;
    int sent;
}frame;

void senddata(int data,int connfd){
    send(connfd,&data,sizeof(data),0);
}


int main(){
    int connfd,n,ack_track;
    struct sockaddr_in address;
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(connfd<0){
        printf("socket creation failed");
        return 0;
    }
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);

    int size=sizeof(address);
    int bytes=connect(connfd,(sa*)&address,size);
    if (bytes<0){
        printf("\nconnection failed");
        return 0;
    }
    printf("\nconnected to server");
    printf("\nenter the number of frames: ");
    scanf("%d",&n);
    senddata(n,connfd);
    frame frames[n];

    for(int i=0;i<n;i++){
        frames[i].data=i+1;
        frames[i].ack=0;
        frames[i].sent=0;
        frames[i].sent_time.tv_sec=0;
    }
    int base=0;
    while(base<n){
        int i=base;
        if(frames[i].sent==0){
            senddata(frames[i].data,connfd);
            printf("\nSending frame %d",frames[i].data);
            frames[i].sent=1;
            gettimeofday(&frames[i].sent_time,NULL);
        }
        if(frames[i].sent==1){
            if(frames[i].ack==1){
                base++;
                continue;
            }
            if(timecheck(frames[i].sent_time.tv_sec)>=TIMEOUT){

                printf("\nResending frame %d",frames[i].data);
                senddata(frames[i].data,connfd);
            }
        }
        sleep(2);
        recv(connfd,&ack_track,sizeof(ack_track),MSG_DONTWAIT);
        if(ack_track==frames[i].data){
            frames[i].ack=1;
            printf("\nAck %d recieved",frames[i].data);
        }
        
    }
    int exitval=-1;
    send(connfd,&exitval,sizeof(exitval),0);
    close(connfd);
    return 0;
}