#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER "255.255.255.255"
#define BUFLEN 512
#define PORT 8888

void die(char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in si_me, si_other;
    int s, i, slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    int broadcast = 1;
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        die("setsockopt (SO_BROADCAST)");
    }

    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        die("setsockopt");
    }

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1) {
        die("bind");
    }

    while(1) {
        printf("Enter message: ");
        fgets(message, BUFLEN, stdin);

        if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, slen)==-1) {
            die("sendto()");
        }
    }

    close(s);
    return 0;
}