/*
   recv.c acts as a receiving server that just binds to the machine's specified port (argv[2]) and writes the data to a specified file (argv[1]). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXSIZE 1000


struct Packet
{
	int done;         // contain 0 if not done, contains 1 if done     
	int seqNo;        // The seqNo used in Stop-And-Wait
	char line[MAXSIZE];  // The next line in the text file
};

int main(int argc, char *argv[])
{
	int s;			   		/* s = socket */
	struct sockaddr_in in_addr;	   	/* Structure used for bind() */
	struct sockaddr_in sock_addr;   	/* Output structure from getsockname */
	struct sockaddr_in src_addr;    	/* Used to receive (addr,port) of sender */
	int src_addr_len;		   	/* Length of src_addr */
	int len;			  	/* Length of result from getsockname */
	struct hostent *host_entry_ptr;


	//check our user input
	if (argc == 1)
	{ 
		printf("Usage: %s <filename> <port>\n", argv[0]);
		exit(1);
	}

	//printf("argv[0] = %s \n", argv[0]);
	//printf("argv[1] = %s \n", argv[1]);
	//printf("argv[2] = %s \n", argv[2]);


	/* ---
	Create a socket
	--- */
	s = socket(PF_INET, SOCK_DGRAM, 0);

	/* ---
	Set up socket end-point info for binding
	--- */
	in_addr.sin_family = AF_INET;                   /* Protocol domain */
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Use wildcard IP address */
	in_addr.sin_port = htons(atoi(argv[2])); 	/* UDP port */

	//printf("in_addr.sin_port = %d \n", in_addr.sin_port);
	
	/* ---
	Here goes the binding...
	--- */
	if (bind(s, (struct sockaddr *)&in_addr, sizeof(in_addr)) == -1)
	{ 
		printf("Error: bind FAILED\n");
		exit(1);
	}
	else
	{ 
		printf("OK: bind SUCCESS\n");
	}

	/* ----
	Check what port I got
	---- */
	len = sizeof(sock_addr);
	getsockname(s, (struct sockaddr *) &sock_addr, &len);
	printf("Socket is bound to:\n");
	printf("  addr = %u\n", ntohl(sock_addr.sin_addr.s_addr));
	printf("  port = %u\n", ntohs(sock_addr.sin_port));

	// open up the file our user specified
	FILE *fp;
	fp = fopen(argv[1], "w+");
	
	// to keep track of our acks
	int seq = 0;
	
	// just gonna sit here and wait...
	while(1)
	{ 
		src_addr_len = sizeof(src_addr);
		struct Packet p;
		
		// keep checking to see if we recieved any packets!
		len = recvfrom(s, (struct Packet *)&p, sizeof(p), 0 /* flags */,
			    (struct sockaddr *) &src_addr, &src_addr_len);
		
		/* ---   
		printf("GOT \n----- \n");
		printf("	p.done = %d \n", p.done);
		printf("	p.seqNo = %d \n", p.seqNo);
		printf("	p.line = %s \n", p.line);
		printf("----- \n");

		printf("	seq = %d \n", seq);
		--- */

		// we got an end packet... send and ack and close out of our file!
		if (p.done == 1)
		{
		
		// stop-and-wait protocol
		while(1)
		{

		   	ssize_t ack = sendto(s, &(p.seqNo), sizeof(p.seqNo), 0 , (struct sockaddr *)&src_addr, sizeof(src_addr));
	
			if(ack==-1)
			{
				perror("send error");
				return EXIT_FAILURE;
			}
			else
			{	
				//ack sent successfully
				/* --- 
				printf("SENT \n----- \n");
			   	printf("	p.done = %d \n", p.done);
			   	printf("	p.seqNo = %d \n", p.seqNo);
			   	printf("	p.line = %s \n", p.line);
			   	printf("----- \n");
			   	--- */
		   	}
		   	
		   	fd_set   recv_fds;                /* I'm using a fd_set var now */
		   	
		   	
		   	// not gonna wait too long
		   	struct timeval timeout; 
			timeout.tv_sec = 0;
			timeout.tv_usec = 5000;

			   /* ===============================
			      Select the bit for socket s
			      =============================== */
			FD_ZERO( &recv_fds );     // Clear all bits in recv_fds
			FD_SET( s, &recv_fds); 	  // Set bit for socket s


			int result ;

			result =  select(FD_SETSIZE, (fd_set *)&recv_fds, NULL, NULL, &timeout);
			//printf("select( ) returns: %d\n", result);
			//printf("ack : %d\n", ack);
	
			//wait for ack	
		   	len = recvfrom(s, (struct Packet *)&p, sizeof(p), 0 ,(struct sockaddr *) &src_addr, &src_addr_len);
		   	
		   	if (result == -1)
		   	{
		   		perror("select() \n");		
				exit(-1);
		   	}
		   	else if (result > 0)
		   	{
		   		// check if we recieved the packet successfully
				if (len <= 0)
				{
					perror("recieve failed");
					exit(-1);
				}
				else
				{
					//Success!
					break;
				}
			}
		   	
		   	else if (result == 0)
		   	{
		   		//Timeout...
		   		continue;
		   	}

		}
		//DONE!!
		//printf("UDP rox my sox!\n");
		fclose(fp);
	}
	
	// check to see if the packet we got is a new word
	else if( p.seqNo == seq )
	{
		//printf("WRITING!");
		fprintf(fp,"%s\n",p.line);
		seq++;
	}
	
	// send an ack with the seqNo that we received
	ssize_t ack = sendto(s, &(p.seqNo), sizeof(p.seqNo), 0 , (struct sockaddr *)&src_addr, sizeof(src_addr));

	if(ack==-1)
	{
		perror("ACK send error");
		return EXIT_FAILURE;
	}
	else
	{
		// Success
		//printf("Sent ACK = %d\n", p.seqNo);

	}


	/* ==================================================== Changed ... */
	host_entry_ptr = gethostbyaddr((char *) &(src_addr.sin_addr.s_addr), 
				     sizeof(src_addr.sin_addr.s_addr), AF_INET);


	}

	close(s);
}
