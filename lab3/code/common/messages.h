#define BLOCKSIZE 1024

struct file_request
{
    uint8_t type; // = 0
    uint8_t filename_size;
    char    *filename;
};

struct ACK
{
    uint8_t type; // = 1
    //TODO Change to original if we plan on implementing Selective ACK
    //uint8_t num_sequences; // For Positive ACK it will always be 1
    //uint16_t *sequence_no;
    uint16_t sequence_no;
};

struct file_info_and_data
{
    uint8_t   type; // = 2
    uint16_t  sequence_number;
    uint8_t   filename_size;
    char      *filename;
    uint32_t  file_size;
    uint16_t  block_size;
    char      data[BLOCKSIZE];
};

struct data
{
    uint8_t   type; // = 3
    uint16_t  sequence_number;
    uint16_t  block_size;
    char      data[BLOCKSIZE];
};

struct file_not_found
{       
    uint8_t  type; // = 4
    uint8_t  filename_size;
    char     *filename;    
};

void send_file_request(struct file_request fr, int s, struct sockaddr_in sin){

	// msgbuf holds actual message
	// outbuf is being used as pointer to fill the locations
	void *msgbuf, *outbuf;

	int total_size = sizeof(fr.type) + sizeof(fr.filename_size) + fr.filename_size;
	msgbuf = outbuf = malloc(total_size);
	memcpy(outbuf, &fr.type, sizeof(fr.type));
	outbuf+=sizeof(fr.type);
	memcpy(outbuf, &fr.filename_size, sizeof(fr.filename_size));
	outbuf+=sizeof(fr.filename_size);
	memcpy(outbuf, fr.filename, fr.filename_size);
	printf("Sending request to server for file: %s\n", fr.filename);

	/* send message to server */  
	sendto(s, msgbuf, total_size, 0, (struct sockaddr *)&sin, sizeof(sin));

}

void send_file_not_found(struct file_not_found fnf, int s, struct sockaddr_storage sin){

    // msgbuf holds actual message
    // outbuf is being used as pointer to fill the locations
    void *msgbuf, *outbuf;

    int total_size = sizeof(fnf.type) + sizeof(fnf.filename_size) + fnf.filename_size;
    msgbuf = outbuf = malloc(total_size);
    memcpy(outbuf, &fnf.type, sizeof(fnf.type));
    outbuf+=sizeof(fnf.type);
    memcpy(outbuf, &fnf.filename_size, sizeof(fnf.filename_size));
    outbuf+=sizeof(fnf.filename_size);
    memcpy(outbuf, fnf.filename, fnf.filename_size);
    printf("Sending message that FNF to client for file: %s\n", fnf.filename);

    /* send message to client */  
    sendto(s, msgbuf, total_size, 0, (struct sockaddr *)&sin, sizeof(sin));

}

void send_file_info_and_data(struct file_info_and_data fiad, int s, struct sockaddr_storage sin){

    // msgbuf holds actual message
    // outbuf is being used as pointer to fill the locations
    void *msgbuf, *outbuf;

    int total_size = sizeof(fiad.type) + sizeof(fiad.sequence_number) + sizeof(fiad.filename_size) 
                        + fiad.filename_size + sizeof(fiad.file_size) + sizeof(fiad.block_size) 
                        + fiad.block_size;

    //sequence_number, file_size, block_size need to be converted to Net Byte order
    fiad.sequence_number = htons(fiad.sequence_number);
    fiad.file_size = htonl(fiad.file_size);
    fiad.block_size = htons(fiad.block_size);

    msgbuf = outbuf = malloc(total_size);
    memcpy(outbuf, &fiad.type, sizeof(fiad.type));
    outbuf+=sizeof(fiad.type);
    memcpy(outbuf, &fiad.sequence_number, sizeof(fiad.sequence_number));
    outbuf+=sizeof(fiad.sequence_number);
    memcpy(outbuf, &fiad.filename_size, sizeof(fiad.filename_size));
    outbuf+=sizeof(fiad.filename_size);
    memcpy(outbuf, fiad.filename, fiad.filename_size);
    outbuf+=fiad.filename_size;
    memcpy(outbuf, &fiad.file_size, sizeof(fiad.file_size));
    outbuf+=sizeof(fiad.file_size);
    memcpy(outbuf, &fiad.block_size, sizeof(fiad.block_size));
    outbuf+=sizeof(fiad.block_size);
    memcpy(outbuf, fiad.data, fiad.block_size);
    printf("Sending message with fiad to client for file: %s\n", fiad.filename);

    /* send message to client */  
    sendto(s, msgbuf, total_size, 0, (struct sockaddr *)&sin, sizeof(sin));

}

void send_data(struct data d, int s, struct sockaddr_storage sin){
    // msgbuf holds actual message
    // outbuf is being used as pointer to fill the locations
    void *msgbuf, *outbuf;

    int total_size = sizeof(d.type) + sizeof(d.sequence_number) + sizeof(d.block_size) 
                        + d.block_size;

    msgbuf = outbuf = malloc(total_size);
    memcpy(outbuf, &d.type, sizeof(d.type));
    outbuf+=sizeof(d.type);
    memcpy(outbuf, &d.sequence_number, sizeof(d.sequence_number));
    outbuf+=sizeof(d.sequence_number);
    memcpy(outbuf, &d.block_size, sizeof(d.block_size));
    outbuf+=sizeof(d.block_size);
    memcpy(outbuf, d.data, d.block_size);

    printf("Sending message with data to client\n");

    /* send message to client */  
    sendto(s, msgbuf, total_size, 0, (struct sockaddr *)&sin, sizeof(sin));
}

void send_ack(struct ACK ack, int s, struct sockaddr_in sin){

    // msgbuf holds actual message
    // outbuf is being used as pointer to fill the locations
    void *msgbuf, *outbuf;

    int total_size = sizeof(ack.type) + sizeof(ack.sequence_no);
    msgbuf = outbuf = malloc(total_size);
    memcpy(outbuf, &ack.type, sizeof(ack.type));
    outbuf+=sizeof(ack.type);
    memcpy(outbuf, &ack.sequence_no, sizeof(ack.sequence_no));
    printf("Sending ACK %d.\n", ntohs(ack.sequence_no)); //Actual data being sent in Network Order

    /* send message to server */  
    sendto(s, msgbuf, total_size, 0, (struct sockaddr *)&sin, sizeof(sin));

}
