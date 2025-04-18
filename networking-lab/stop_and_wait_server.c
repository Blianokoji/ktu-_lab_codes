
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 5000
#define sa struct sockaddr
#define loss 30

int loss_sim(){
    srand(time(NULL));

    if((rand()%100)<loss)
        return 0;
    return 1;
}

void ack(int data,int connfd){
    send(connfd,&data,sizeof(data),0);
    printf("\nsending ack %d",data);
}


int main(){
    int connfd,sockfd;
    int n;
    struct sockaddr_in address;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        printf("socket creation failed");
        return 0;
    }

    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);

    int size=sizeof(address);
    if (bind(sockfd,(sa*)&address,size)<0){
        return 0;
    }
    printf("\nbind successful");
    printf("\nlistening...");
    listen(sockfd,3);
    connfd=accept(sockfd,(sa*)&address,&size);
    recv(connfd,&n,sizeof(n),0);
    int seq=1;
    while(1){
        int data;
        recv(connfd,&data,sizeof(data),0);
        if(data==-1){
            printf("\nfinished...");
            exit(0);
        }
        if(loss_sim()){
            sleep(2);
            printf("\nframe %d recieved ",data);
            ack(data,connfd);
                
        }
    }
    close(sockfd);
    close(connfd);
   return 0;

}