/* 
    CSD 304 Computer Networks, Fall 2016
    Lab 3, server
    Group 4:
        Manjul Singh Sachan     (1410110228)
        Manvendra Singh         (1510110507)
        Mohak Garg              (1410110247)
        Rohan Verma             (1510110508)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include "../common/messages.h"

#define SERVER_PORT 5432
#define BUF_SIZE 90960

int main(int argc, char * argv[])
{

    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[BUF_SIZE];
    int s, len;
    uint8_t rec_type; 

    if ((argc==2)||(argc == 3)) 
    {
        host = argv[1];
    }
    else 
    {
        fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
        exit(1);
    }

    if(argc == 3) 
    {
        fp = fopen(argv[2], "w");
        if (fp == NULL) 
        {
            fprintf(stderr, "Error opening output file\n");
            exit(1);
        }
    }

    /* translate host name into peer's IP address */
    hp = gethostbyname(host);
    if (!hp) 
    {
        fprintf(stderr, "client: unknown host: %s\n", host);
        exit(1);
    }
    else
        printf("Host %s found!\n", argv[1]);

    /* build address data structure */
    memset((char *)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);


    /* create socket */
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("client: socket");
        exit(1);
    }

    // TODO: Set using argument argv
    char *filename = "sia.m4a";

    struct file_request fr;
    fr.type = 0;
    fr.filename_size = strlen(filename);
    fr.filename = filename;

    send_file_request(fr, s, sin);

    /* wait for reply of file request and respond */
    printf("Waiting for response from server\n");
    while( len = recv(s, buf, sizeof(buf), 0) > 0){ 
        memcpy(&rec_type, buf, 1);
        printf("Type: %d\n", rec_type);
        if(rec_type == 4){
            printf("File was not found on the server!\n");
        }
        else if(rec_type == 2){
            printf("File was found\n");
        }
    }
    
    return 0;
}

