#include <stdio.h> 
#include <sys/socket.h> /* socket, connect, socklen_t */
#include <arpa/inet.h> /* sockaddr_in, inet_pton */
#include <string.h> 
#include <stdlib.h> /* atoi */
#include <fcntl.h> /* O_WRONLY, O_CREAT */
#include <unistd.h> /* close, write, read */
#define SRV_PORT 5322
#define MAX_RECV_BUF 256
#define MAX_SEND_BUF 256

int recv_file(int sock, char* file_name)
{
 	char send_str [MAX_SEND_BUF]; /* message to be sent to server*/
 	int f; /* file handle for receiving file*/
	ssize_t sent_bytes, rcvd_bytes, rcvd_file_size;
 	int recv_count; /* count of recv() calls*/
 	char recv_str[MAX_RECV_BUF]; /* buffer to hold received data */
 	size_t send_strlen; /* length of transmitted string */

 	sprintf(send_str, "%s\n", file_name); /* add CR/LF (new line) */
 	send_strlen = strlen(send_str); /* length of message to be transmitted */
	if( (sent_bytes = send(sock, file_name, send_strlen, 0)) < 0 ) {
		perror("send error");
		return -1;
	}
 	/* attempt to create file to save received data. 0644 = rw-r--r-- */
	if ( (f = open(file_name, O_WRONLY|O_CREAT, 0644)) < 0 )
	{
		perror("error creating file");
		return -1;
	}

 	recv_count = 0; /* number of recv() calls required to receive the file */
 	rcvd_file_size = 0; 

 	int keep_file = 1;
	while ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) > 0 )
	{
		recv_count++;
		rcvd_file_size += rcvd_bytes;
		//printf("%d", strcmp(recv_str, "FNF\n") == 0);
		if(strcmp(recv_str, "FNF\n") == 0){
			printf("File not found\n");
			keep_file = 0;
			break;
		}
		else if(keep_file == 1){
			printf("OK\n");
			keep_file = 2;
		}

		if (write(f, recv_str, rcvd_bytes) < 0 )
		{
			perror("error writing to file");
			return -1;
		}
	}
 	close(f); /* close file*/
 	if(!keep_file) remove(file_name);
 	else{
		printf("I have Received: %d bytes in %d recv(s)\n", rcvd_file_size, recv_count);
	}
	return rcvd_file_size;
}

int main(int argc, char* argv[])
{

		int sock_fd;
		struct sockaddr_in srv_addr;

		if (argc < 2)
		{
			printf("usage: %s <IP address> [port number]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	 	memset(&srv_addr, 0, sizeof(srv_addr)); /* zero-fill srv_addr structure*/
	for(;;){
	 	/* create a client socket */
		sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	 	srv_addr.sin_family = AF_INET; /* internet address family */

	 	/* convert command line argument to numeric IP */
		if ( inet_pton(AF_INET, argv[1], &(srv_addr.sin_addr)) < 1 )
		{
			printf("Invalid IP address\n");
			exit(EXIT_FAILURE);
		}

	 	/* if port number supplied, use it, otherwise use SRV_PORT */
		srv_addr.sin_port = (argc > 2) ? htons(atoi(argv[2])) : htons(SRV_PORT);

		if( connect(sock_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0 )
		{
			perror("connect error");
			exit(EXIT_FAILURE);
		}
		printf("HELLO\nconnected to:%s:%d ..\n",argv[1],SRV_PORT);

		char file_name[100];
		scanf("%s", &file_name);
		if(strcmp(file_name, "Bye") == 0) break;
	 	recv_file(sock_fd, file_name); /* argv[1] = file name */

	 	/* close socket*/
		if(close(sock_fd) < 0)
		{
			perror("socket close error");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
