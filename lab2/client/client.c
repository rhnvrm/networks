/* CSD 304 Computer Networks, Fall 2016
   Lab 2, client
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
#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 5432
#define BUF_SIZE 40960

int delay_nano(long t1, long t2);

int main(int argc, char * argv[]){
  
  FILE *fp;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[BUF_SIZE];
  int s;
  int len;

  if ((argc==2)||(argc == 3)) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
    exit(1);
  }

  if(argc == 3) {
    fp = fopen(argv[2], "w");
    if (fp == NULL) {
      fprintf(stderr, "Error opening output file\n");
      exit(1);
    }
  }

  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
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
  
  printf("Client will get data from to %s:%d.\n", argv[1], SERVER_PORT);
  printf("To play the music, pipe the downlaod file to a player, e.g., ALSA, SOX, VLC: cat recvd_file.wav | vlc -\n"); 
  
  
  

  /* send message to server */  
  fgets(buf, sizeof(buf), stdin);
  buf[BUF_SIZE-1] = '\0';
  len = strlen(buf) + 1;
  if (sendto(s, buf, len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) { 
    perror("Client: sendto()"); 
    return 0; 
  }
    
  /* get reply, display it or store in a file*/ 
  /* Add code to receive unlimited data and either display the data
     or if specified by the user, store it in the specified file. 
     Instead of recv(), use recvfrom() call for receiving data */
  //int fpt = open("recv", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    FILE *fpt = fopen("recv","w");
    while( (len = recv(s, buf, sizeof(buf), 0)) > 0){ 
     if (strcmp(buf, "BYE") == 0) {
      printf("BYE\n");
      break;
     }
    fwrite(buf,1,len,fpt);
    	//printf(buf);
     fflush(fpt);
   }
	fclose(fpt);
  //fputs(buf, stdout);
  
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

   printf("Nano sleep successfull \n");

   return 0;
}
