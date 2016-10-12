struct file_request
{
    uint8_t type; // = 0
    uint8_t filename_size;
    char    *filename;
};

struct ACK
{
    uint8_t type; // = 1
    uint8_t num_sequences;
    uint16_t *sequence_no;
};

struct file_info_and_data
{
    uint8_t   type; // = 2
    uint16_t  sequence_number;
    uint8_t   filename_size;
    char      *filename;
    uint32_t  file_size;
    uint16_t  block_size;
    char      *data;
};

struct data
{
    uint8_t   type; // = 3
    uint16_t  sequence_number;
    uint16_t  block_size;
    char      *data;
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



