/*
	send.c binds to a specified port (argv[3]) on some IP address (argv[2]) and reads data from a specified file (argv[1]). 
	
 	- sendto() uses UDP and I implement a Stop-and-Wait ACK protocol to ensure correct transmission
 	- uses symbolic name for address (lab1a)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>

#define MAXSIZE 1000


struct Packet
{
	int done;         // contain 0 if not done, contains 1 if done     
	int seqNo;        // The seqNo used in Stop-And-Wait
	char line[MAXSIZE];  // The next line in the text file
};



int main(int argc, char *argv[])
{
	int s;				/* s = socket */
	struct sockaddr_in in_addr;		/* Structure used for bind() */
	struct sockaddr_in sock_addr;	/* Output structure from getsockname */
	struct sockaddr_in dest_addr;	/* Destination socket */
	int len, ack, count = 0;

	struct hostent *host_entry_ptr;     /* Structure to receive output */

	//check input
	if (argc < 5)
	{ 
		printf("Usage: %s <filename = %s > <hostname =  %s > <port# = %s > <timeout = %s >\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
		exit(1);
	}

	/* -------
	Fill in destination socket - this will identify IP-host + (UDP) port
	------- */
	dest_addr.sin_family = AF_INET;		 /* Internet domain */

	host_entry_ptr = gethostbyname(argv[2]);   /* E.g.: cssun.mathcs.emory.edu */

	if (host_entry_ptr == NULL)
	{ 
		printf("Host `%s' not found...\n", argv[2]);     /* Just for safety.... */
		exit(1);
	}

	/**********************************************************************
	* NOTE: DO NOT use htonl on the h_addr_list[0] returned by 
	* gethostbyname() !!!
	**********************************************************************/
	memcpy((char *) &(dest_addr.sin_addr.s_addr), 
		   host_entry_ptr->h_addr_list[0], 
		   host_entry_ptr->h_length);

	/**********************************************************************/

	dest_addr.sin_port = htons(atoi(argv[3]));    /* Destination (UDP) port # */

	/* ---
	Create a socket
	--- */
	s = socket(PF_INET, SOCK_DGRAM, 0);


	/* ---
	Set up socket end-point info for binding
	--- */
	memset((char *)&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;                   /* Protocol domain */
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Use wildcard IP address */
	in_addr.sin_port = htons(0);	           	   /* Use any UDP port */

	/* ---
	Here goes the binding...
	--- */
	if (bind(s, (struct sockaddr *)&in_addr, sizeof(in_addr)) == -1)
	{ 
		printf("Error: bind FAILED\n");
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
	printf("Socket s is bind to:\n");
	printf("  addr = %u\n", ntohl(sock_addr.sin_addr.s_addr));
	printf("  port = %d\n", ntohs(sock_addr.sin_port));

	//open up the file our user asked us to read
	FILE *fp = fopen( argv[1], "r");
	
	/* ----
	Stuff we'll need later to send packets
	---- */
	
	// timeval for select()
	struct timeval timeout; 
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000*atoi(argv[4]);
	
	// how we will keep track of the words
	int seq = 0;
	
	// the memory where we will keep all our precious data!
	char buf[MAXSIZE];
	
	// time to get to business... keep reading until EOF
	while ( ( fscanf(fp, "%s\n", buf) > 0 ) )
	{
	
	// assemble the data in our Packet struct
	struct Packet p;
	p.done = 0;
	p.seqNo = seq;
	memcpy(p.line, buf, sizeof(buf)+1);
	memset(buf, 0, sizeof(buf));
	seq++;

	while(1){

	   	off_t m = sendto(s, (struct Packet *)&p, sizeof(p)+1, 0 , (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	   	count++;
	
		if(m==-1)
		{
			perror("send error");
			return EXIT_FAILURE;
		}
		else
		{
			/* ---
			printf("SENT \n----- \n");
		   	printf("	p.done = %d \n", p.done);
		   	printf("	p.seqNo = %d \n", p.seqNo);
		   	printf("	p.line = %s \n", p.line);
		   	printf("----- \n");
		   	---- */
	   	}
	   	
	   	fd_set   recv_fds;                /* I'm using a fd_set var now */
	   	timeout.tv_sec = 0;
		timeout.tv_usec = 1000*atoi(argv[4]);

	        /* ===============================
	           Select the bit for socket s
	        =============================== */
		FD_ZERO( &recv_fds );     // Clear all bits in recv_fds
		FD_SET( s, &recv_fds); 	  // Set bit for socket s


		int result ;

		result =  select(FD_SETSIZE, (fd_set *)&recv_fds, NULL, NULL, &timeout);
		//printf("select( ) returns: %d\n", result);
		//printf("ack : %d\n", ack);

	   	if (result == -1)
	   	{
	   		perror("select() \n");		
			break;
	   	}
	   	else if (result > 0)
	   	{
	   		//wait for ack	
	
	   		socklen_t addr_len = sizeof dest_addr;
			int rec = recvfrom(s, &ack, sizeof(ack), 0 , (struct sockaddr *)&dest_addr, &addr_len);
		
			if (rec <= 0)
			{
				perror("recieve failed");
				exit(-1);
			}
	   		if(ack == p.seqNo)
	   		{
	   			//ACK is correct!
	   			break;
	   		}
	   		else
	   		{
	   			//ACK is incorrect...
	   			//Resend!
	   			
	   			continue;
	   		}
	   	}
	   	else if (result == 0)
	   	{
	   		//Timeout...
	   		continue;
	   	}
		

	}

	}
	//DONE with file Transfer!
	
	// create an end packet so recv knows we're done transmitting...
	struct Packet end;
	end.done = 1;
	end.seqNo = 0;
	strcpy(end.line,"<3");

	off_t z = sendto(s, (struct Packet *)&end, sizeof(end)+1, 0 , (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	count++;
	
	if(z==-1)
	{
		perror("send error");
		return EXIT_FAILURE;
	}
	else
	{
		/* ---
		printf("SENT \n----- \n");
	   	printf("	end.done = %d \n", end.done);
	   	printf("	end.seqNo = %d \n", end.seqNo);
	   	printf("	end.line = %s \n", end.line);
	   	printf("----- \n");
	   	---- */
	}

	
	fd_set   recv_fds; 
	FD_ZERO( &recv_fds );     // Clear all bits in recv_fds
	FD_SET( s, &recv_fds); 	  // Set bit for socket s
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000*atoi(argv[4]);

	int result ;


	result =  select(FD_SETSIZE, (fd_set *)&recv_fds, NULL, NULL, &timeout);

	if (result == -1)
	{
		perror("select() \n");		
		exit(-1);
	}
	while (result == 0)
	{
	
		//printf("TIMEOUT \n");	
		//RESEND
		z = sendto(s, (struct Packet *)&end, sizeof(end)+1, 0 , (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		count++;
		
		if(z==-1)
		{
			perror("resend error");
			return EXIT_FAILURE;
		}
		
	
		FD_ZERO( &recv_fds );     // Clear all bits in recv_fds
		FD_SET( s, &recv_fds); 	  // Set bit for socket s
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000*atoi(argv[4]);
	   	
	   	result =  select(FD_SETSIZE, (fd_set *)&recv_fds, NULL, NULL, &timeout);
	}
	
	
	//printf("total packet send count = %d \n", count);


	fclose(fp);
	close(s);
}


 



