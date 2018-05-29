/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#include "computer.h"

int messageQID, sharedMID,procIndex;
sharedMemory *mem;
message msg;


//find an integer index of i in the bitmap
int whichInt(int i) {
	return ((i - 2) / 32);
}

//find the bit position within the integer index of i in the bitmap
int whichBit(int i) {
	return ((i - 2) % 32);
}

//set bit in bitmap to checked
int setBitTo1(int integer, int bit)
{

	mem->bitmap[integer] |= (1 << bit);
	
	return 1;
}

//calculate perfect numbers
void perfect(int start) 
{
	int currInt, currBit, i, sum,first;
	int n = start;
	
	bool wrappedAround;
	
	
	while (1) {
	
		//wrap around if necessary
		if(n > 33554432)
		{
			//printf("Wrapping around \n");
			wrappedAround = true;
			n = 2;
			first = 0;
			
			if(n >= start) { printf("We've been to the moon and back! \n"); exit(0);}
		}
		
		//determine bitmap coordinates
		currInt = whichInt(n);
		currBit = whichBit(n);
		
		//only checking numbers that have not already been checked
		if ((mem->bitmap[currInt] & (1 << currBit)) == 0)
		{
			if(first == 0){
				//printf("First checked: %d \n",n);
				first++;
			}
			
			mem->processList[procIndex].numTested++;
			
			sum=1;
			for (i=2;i<n;i++) {if (!(n%i)) sum+=i;}

			//found a perfect number!
			if (sum==n)
			{ 
				//printf("%d is perfect\n",n); 
				mem->processList[procIndex].numFound++;
		
				//send a message for the perfect number!
				msg.type = PERFECTION;
				msg.body = n;		
				if(msgsnd(messageQID, &msg, sizeof(msg.body), 0) != 0)
				{
					perror("Failed to send perfect number message");
					exit(-1);
				}
			}
		}else {
			//number has already been checked. updated the shared memory
			mem->processList[procIndex].numSkipped++;
			//printf("Process %d has now skipped %d numbers (n = %d)\n",procIndex, mem->processList[procIndex].numSkipped,n);
		}
		
		if(setBitTo1(currInt,currBit) != 1)
		{
			printf("There was a problem setting the bitmap.");
			exit(0);
		}
		n++;
	}
}

static void terminate(int signum)
{	
	mem->processList[procIndex].pid = 0;
	mem->processList[procIndex].numFound = 0;
	mem->processList[procIndex].numTested = 0;
	mem->processList[procIndex].numSkipped = 0;
	
	exit(0);
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		printf("Please specify a starting number. \nUSAGE: ./compute [starting number]");
		exit(-1);
	}
	
	int startNum = atoi(argv[1]);
	
	//attach to shared memory segment
	sharedMID = shmget(SHAREDM_KEY,sizeof(sharedMemory), 0);
	if(sharedMID == -1)
	{
		perror("Unable to find shared memory");
		exit(-1);
	}
	
	mem = shmat(sharedMID, NULL,0);
	
	//attach to message queue
	messageQID = msgget(MESSAGEQ_KEY,0);
	if(messageQID == -1)
	{
		perror("Unable to find message queue");
		exit(-1);
	}
	
	int currPID = getpid();
	
	//send a request to manage with te current PID
	msg.type = GET_INDEX;
	msg.body = currPID;
	if(msgsnd(messageQID, &msg, sizeof(msg.body), 0) != 0)
	{
		perror("Failed to send currPID message");
		exit(-1);
	}
	
	//setup a new sigaction
	struct sigaction act = {};
	memset(&act,0,sizeof(act));
	sigemptyset(&act.sa_mask);
    	act.sa_flags = 0;
    	
    	//setup signal handler
	act.sa_handler = terminate;
  	
	if(sigaction(SIGINT, &act, NULL) != 0) 
	{
		perror("SIGINT failed");
		exit(1);
	}
	if(sigaction(SIGHUP, &act, NULL) != 0) 
	{
		perror("SIGHUP failed");
		exit(1);
	}
	if(sigaction(SIGQUIT, &act, NULL) != 0) 
	{
		perror("SIGQUIT failed");
		exit(1);
	}
	
	//wait for confirmation message
	msgrcv(messageQID,&msg,sizeof(msg.body),INDEX_SUCCESS,0);
	
	procIndex = msg.body;
	//printf("Index for this process is: %d\n", procIndex);
	
	//successfully attached to manage! time to start computing.
	perfect(startNum);
	
}
