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
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>

#include "../common/messages.h"

#define SERVER_PORT 5432
#define BUF_SIZE 90960
#define BLOCKSIZE 1024

int main(int argc, char * argv[])
{
	struct sockaddr_in sin;
	struct sockaddr_storage client_addr;
    char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
	socklen_t client_addr_len;
	char buf[BUF_SIZE];
	int len, s;
	char *host;
	struct hostent *hp;
    uint8_t rec_type; 
    char* filename;
    FILE* f;

    // Create Socket 
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
     	perror("server: socket");
     	exit(1);
    }

    // build address data structure and bind to all local addresses
    memset((char *)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    /* If socket IP address specified, bind to it. */
    if(argc == 2) 
    {
        host = argv[1];
     	hp = gethostbyname(host);
     	if (!hp) 
        {
     		fprintf(stderr, "server: unknown host %s\n", host);
     		exit(1);
     	}
     	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
    }
    else
    {
        /* Else bind to 0.0.0.0 */  
        sin.sin_addr.s_addr = INADDR_ANY;
    }
    sin.sin_port = htons(SERVER_PORT);

    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) 
    {
     	perror("server: bind");
     	exit(1);
    }
    else
    {
        /* Add code to parse IPv6 addresses */
     	inet_ntop(AF_INET, &(sin.sin_addr), clientIP, INET_ADDRSTRLEN);
     	printf("Server is listening at address %s:%d\n", clientIP, SERVER_PORT);
    }

    printf("Client needs to send \"GET\" to receive the file %s\n", argv[1]);


    client_addr_len = sizeof(client_addr);



    /* Receive messages from clients*/
    while(len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len))
    {

     	inet_ntop(client_addr.ss_family,
     		&(((struct sockaddr_in *)&client_addr)->sin_addr),
     		clientIP, INET_ADDRSTRLEN);

        printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);
        memcpy(&rec_type, buf, 1);
        printf("Type: %d\n", rec_type);
        if(rec_type == 0)
        {
            // File Request
            // Parse rest of the data inside
            uint8_t filename_size;
            memcpy(&filename_size, buf+1, 1);
            printf("Filename Size: %d\n", filename_size);
            filename = malloc(filename_size);
            memcpy(filename, buf+2, filename_size);
            printf("Filename: %s\n", filename);


            // Check if file exists
            FILE* f = fopen(filename, "r");
            if(f == NULL)
            {
                // Send File Not Found
                struct file_not_found fnf;
                fnf.type = 4;
                fnf.filename_size = strlen(filename);
                fnf.filename = filename;
                send_file_not_found(fnf, s, client_addr);
            }
            else{
                // Set Filename and initialze sliding window parameters
                struct file_info_and_data fiad;
                fiad.type = 2;
                fiad.sequence_number = 0;
                fiad.filename_size = strlen(filename);
                fiad.filename = filename;
                fseeko(f, 0L, SEEK_END);
                fiad.file_size = ftello(f);
                fseeko(f, 0, SEEK_SET);
                fiad.block_size = BLOCKSIZE;
                fread(fiad.data,1,fiad.block_size,f);
                send_file_info_and_data(fiad, s, client_addr);
                fclose(f);
            }   
            // Optional: Add code to serve multiple clients by forking (Reuse from LAB#2)
        }
        else if(rec_type == 1){
            // ACK 
            uint16_t ack_seq_no;
            memcpy(&ack_seq_no, buf+1, 2);
            ack_seq_no = ntohs(ack_seq_no); // Convert from Net to Host byte order
            printf("ACK %d received\n", ack_seq_no);
            // Update Sliding Window Parameters
        }
        /* Send to client */
        
    }


    return 0;
}
