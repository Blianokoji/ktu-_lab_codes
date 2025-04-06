#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define PORT 8000
#define MAX_FRAMES 100
#define TIMEOUT 3
#define LOSS_PROBABILITY 30  // percentage chance to lose a packet to simulate packet loss
#define SA struct sockaddr

struct frame {
    int seq; //sequence number 
    int acked; //ack status
    int sent; //whether sent 
    struct timeval sent_time; //timeout
};// this is the structure for a single frame 

int n, window_size;
struct frame frames[MAX_FRAMES];

int should_drop() {
    return rand() % 100 < LOSS_PROBABILITY;
}// loss

void send_frame(int connfd, int i) {
    if (should_drop()) {
        printf("\nFRAME %d lost (simulated)", frames[i].seq);
        frames[i].sent = 1;
        gettimeofday(&frames[i].sent_time, NULL); // ssets the current time into timeval struct, here, the timeout
        return;
    }

    send(connfd, &frames[i].seq, sizeof(int), 0);
    gettimeofday(&frames[i].sent_time, NULL);
    frames[i].sent = 1;
    printf("\nFRAME %d sent", frames[i].seq);
}//this is the function to send a frame 

int time_elapsed(struct timeval t) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - t.tv_sec);
}

int main() {
    srand(time(0)); //random generator based on current time , this is so that every time we get different random value

    int connfd;
    struct sockaddr_in servaddr;

    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (connect(connfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to server failed\n");
        exit(1);
    }

    printf("Enter number of frames: ");
    scanf("%d", &n);
    printf("Enter window size: ");
    scanf("%d", &window_size);

    for (int i = 0; i < n; i++) {
        frames[i].seq = i + 1;
        frames[i].acked = 0;
        frames[i].sent = 0;
        frames[i].sent_time.tv_sec = 0;
    }

    int base = 0;

    while (base < n) {
        for (int i = base; i < base + window_size && i < n; i++) { // essentially the base can go upto n-1 serving all the frames and i stays in the same range  , to make sure of this we used i<n also
            if (!frames[i].acked && !frames[i].sent) {
                send_frame(connfd, i); //if neither acked nor sent then send 
            } else if (!frames[i].acked && time_elapsed(frames[i].sent_time) >= TIMEOUT) { // if no ack and time out reached then retransmit
                printf("\nTIMEOUT for frame %d. Retransmitting...", frames[i].seq);
                send_frame(connfd, i);
            }
        }

        int ack;
        while (recv(connfd, &ack, sizeof(int), MSG_DONTWAIT) > 0) {
            if (ack > 0 && ack <= n && !frames[ack - 1].acked) {
                frames[ack - 1].acked = 1; // index of 1 is 0 so when we get ack we have to do ack -1 to get its frame index , basially if no ack then ack 
                printf("\nACK %d received", ack);
            }
        }

        while (base < n && frames[base].acked)//if acked frames exist just slide the window through 
            base++;

        usleep(300000); // 300ms , simple delay before each iteration for everything to go smoothly
    }

    close(connfd);
    return 0;
}
