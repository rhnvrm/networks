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
#include <inttypes.h>        

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
    int *ack_counter;
    int lfr, laf, rws;

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
    // char *filename = "warbitch.txt";

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
            // RECV FIAD
            printf("File was found\n");

            //Parse the initial FIAD 
            uint16_t rec_seq_no;
            memcpy(&rec_seq_no, buf+1, 2);
            rec_seq_no = ntohs(rec_seq_no); // Convert from Net to Host byte order
            uint8_t rec_fname_size;
            memcpy(&rec_fname_size, buf+3, 1);
            printf("Filename Size: %d\n", rec_fname_size);
            filename = malloc(rec_fname_size);
            memcpy(filename, buf+4, rec_fname_size);
            printf("Filename: %s\n", filename);
            uint32_t rec_file_size;
            memcpy(&rec_file_size, buf+4+rec_fname_size, 4);
            rec_file_size = ntohl(rec_file_size);
            printf("Filesize: %d Bytes\n", rec_file_size);
            uint16_t block_size;
            memcpy(&block_size, buf+4+rec_fname_size+4, 2);
            block_size = block_size;
            printf("block_size: %d Bytes\n", block_size);
            char rec_data[BLOCKSIZE];
            memcpy(&rec_data, buf+4+rec_fname_size+4+2, block_size);
            //printf("DATA: %s\n", rec_data);
            //Open file and write to the file
            //Discard this Data

            //set SWP parameters
            unsigned long calc_frame_size = rec_file_size/block_size;
            printf("Calculated frames = %ld\n", calc_frame_size);

            lfr = -1;
            rws = 1; //Anything goes? RWS = SWS
            laf = 0;

            //Send initial ACK
            struct ACK ack = {1, rec_seq_no};
            send_ack(ack, s, sin);
        }
        else if(rec_type == 3)
        {
            // RECV Data
            uint16_t rec_seq_no;
            memcpy(&rec_seq_no, buf+1, 2);
            rec_seq_no = ntohs(rec_seq_no); // Convert from Net to Host byte order
            printf("Seq No: %"PRIu16"\n", rec_seq_no);
            
            if(rec_seq_no == lfr+1) {
                lfr++;
                // sending ack 
                struct ACK ack = {1, rec_seq_no};
                send_ack(ack, s, sin);
                laf++;
                uint16_t block_size;
                memcpy(&block_size, buf+3, 2);
                block_size = block_size;
                printf("block_size: %d Bytes\n", block_size);
                char rec_data[BLOCKSIZE];
                memcpy(&rec_data, buf+5, block_size);
                //printf("DATA: %s\n", rec_data);
                FILE *fp = fopen(filename, "a");
                fprintf(fp,"%s", rec_data);
                fclose(fp);    
            }
            


        }
    }
    
    return 0;
}

