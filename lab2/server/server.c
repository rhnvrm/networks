/* CSD 304 Computer Networks, Fall 2016
   Lab 2, server
   Team 4:
   Manjul Singh Sachan (1410110228)
   Manvendra Singh (1510110507)
   Mohak Garg (1410110247)
   Rohan Verma (1510110508)
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
#include <sys/time.h> // for measuring network speed

#define SERVER_PORT 5432
#define BUF_SIZE 90960

/*
Delay funtion to control the data rate
*/
int delay_nano(long t1, long t2); 

int main(int argc, char * argv[]){
	struct sockaddr_in sin;
	struct sockaddr_storage client_addr;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
	socklen_t client_addr_len;
	char buf[BUF_SIZE];
	int len;
	int s;
	char *host;
	struct hostent *hp;
	int f;
	char * filename;
	long int payload_size;
	ssize_t length;
	long sleep;

	/* for measuring data transfer speed */
	struct timeval t1, t2;
	double elapsedTime;
	long int file_size;
	double data_rate;

  /* 
     Command line arguments:
     @params:
     - nanosleep : long 
     - payload_size : int
     - name of the file to be sent : char*
  */
     if(argc >= 2) {
     	sleep =  atol(argv[1]);
     	printf("sleep interval has been set to %ld ns\n", sleep);
     }

     if(argc >= 3) {
     	payload_size = atol(argv[2]);
     	printf("payload_size size has been set to %ld \n \n",payload_size );
     }

     if(argc >= 4) {
    // file name gets stored in argv[3]
     	filename = argv[3];
     	printf("file to be sent: %s \n", argv[3]);
     } 

  /* Create a socket */
     if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
     	perror("server: socket");
     	exit(1);
     }

  /* build address data structure and bind to all local addresses*/
     memset((char *)&sin, 0, sizeof(sin));
     sin.sin_family = AF_INET;

  /* If socket IP address specified, bind to it. */
     if(argc == 2) {
     	host = argv[1];
     	hp = gethostbyname(host);
     	if (!hp) {
     		fprintf(stderr, "server: unknown host %s\n", host);
     		exit(1);
     	}
     	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
     }
  /* Else bind to 0.0.0.0 */
     else
     	sin.sin_addr.s_addr = INADDR_ANY;

     sin.sin_port = htons(SERVER_PORT);

     if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
     	perror("server: bind");
     	exit(1);
     }
     else{
    /* Add code to parse IPv6 addresses */
     	inet_ntop(AF_INET, &(sin.sin_addr), clientIP, INET_ADDRSTRLEN);
     	printf("Server is listening at address %s:%d\n", clientIP, SERVER_PORT);
     }

     printf("Client needs to send \"GET\" to receive the file %s\n", argv[1]);


     client_addr_len = sizeof(client_addr);



  /* Receive messages from clients*/
     while(len = recvfrom(s, buf, sizeof(buf), 0,
     	(struct sockaddr *)&client_addr, &client_addr_len)){

     	inet_ntop(client_addr.ss_family,
     		&(((struct sockaddr_in *)&client_addr)->sin_addr),
     		clientIP, INET_ADDRSTRLEN);

     printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);


     pid_t b;
    /* Send to client */
     if(strcmp("GET\n", buf) == 0){
     	/* fork if client sends GET */
     	b = fork();


     	if (b < -1) { 
     		perror("Fork failed");
     		exit(1);
     	}


     	else if(b > 0 ) {
     		printf ("In the parent process, with id %d\n", (int) getppid ());
     		printf ("In child process's ID is %d\n",(int) b);
     	}



     	else if(b == 0) { 
         /*
         	- child process created to service the client request.
         	- spawned every time a new client send GET
         	- will send the file and exit()
         */

         	printf("In child process\n");

         	FILE *fpt = fopen(filename,"r");
         	// initialise timer
         	gettimeofday(&t1, NULL);

         	while((length = fread(buf,1,payload_size,fpt) ) > 0){ 

         		if ((len = sendto(s, buf, length, 0,
         			(struct sockaddr *)&client_addr, client_addr_len)) == -1) {
         			perror("server: sendto");
         		exit(1);
         	}
         	file_size += len;
         	//printf("length of buffer is %d \n", len);
         	delay_nano(0, sleep);
         }
      /* Send BYE to signal termination */
         printf("BYE \n");
         strcpy(buf, "BYE");
         sendto(s, buf, sizeof(buf), 0, 
         	(struct sockaddr*)&client_addr, client_addr_len);

         memset(buf, 0, sizeof(buf));
         //stop timer
         gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
    data_rate = (file_size/elapsedTime)/1024;
    printf("time elapsed is %lf seconds.\n", elapsedTime);
    printf("data rate is %lf KBps\n",data_rate);


    exit(0);
}
}
printf("Parent listening...\n");
continue;
}

}

int delay_nano(long t1, long t2){
	struct timespec tim, tim2;
	tim.tv_sec = t1;
	tim.tv_nsec = t2;

	if(nanosleep(&tim , &tim2) < 0 )   
	{
		printf("Nano sleep system call failed \n");
		return -1;
	}

	return 0;
}
