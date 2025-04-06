#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define PORT 8000
#define MAX_FRAMES 100
#define TIMEOUT_SEC 5
#define SA struct sockaddr

int received[MAX_FRAMES] = {0}; //to keep track of recieved frames
int n;

struct timeval frame_time[MAX_FRAMES]; //this is for time stamping , check readme for info

int time_elapsed(struct timeval start) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec);
}//same as client to calculate time elapsed :: current time - sent time of frame/start time

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed\n");
        exit(1);
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed\n");
        exit(1);
    }

    len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Server accept failed\n");
        exit(1);
    }

    printf("Server connected to client...\n");

    printf("Enter number of frames to receive: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        frame_time[i].tv_sec = 0; // mark as not yet received
    }

    int total_received = 0;

    while (total_received < n) {
        int seq;
        int bytes = recv(connfd, &seq, sizeof(int), MSG_DONTWAIT);

        if (bytes > 0) {//if we did recieve something then do:
            if (seq >= 1 && seq <= n) {
                if (!received[seq - 1]) {
                    received[seq - 1] = 1;
                    gettimeofday(&frame_time[seq - 1], NULL);
                    total_received++;
                    printf("FRAME %d received\n", seq);
                } else {
                    printf("Duplicate FRAME %d received\n", seq);
                }

                // send ack
                send(connfd, &seq, sizeof(int), 0);
                printf("ACK %d sent\n", seq); // sewnds ack if regular frame received or even if duplicate frame received
            }
        }

        // check for any frames not received yet, and flag if timed out
        for (int i = 0; i < n; i++) {
            if (!received[i] && frame_time[i].tv_sec == 0) {
                gettimeofday(&frame_time[i], NULL); // start timer
            }
            if (!received[i] && time_elapsed(frame_time[i]) >= TIMEOUT_SEC) {
                printf("Still waiting for FRAME %d (possible loss or delay)...\n", i + 1);
                frame_time[i].tv_sec = 999999; // prevent repeat logging
            }
        }// if the whole thing takes too long then obv smtng wrong so we check if evrything is under the timeout as the next usleep is putting a .2s delay so there will be time taken

        usleep(200000); // 0.2s pause, we didnt use sleep because it takes too long , yoou can use that too
    }

    printf("All frames received. Server exiting.\n");
    close(connfd);
    close(sockfd);
    return 0;
}
