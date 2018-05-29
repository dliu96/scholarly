#include <stdio.h>
/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu
*/

#include <stdlib.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#include "computer.h"

int messageQID, sharedMID;
sharedMemory *mem;

static void apocalypse(int signum)
{
	int pid;
	
	printf("Hello world! \n");
	
	for(int i = 0; i < 20; i++)
	{
		pid = mem->processList[i].pid;
		
		if (pid != 0)
		{
			printf("Killing process %d \n",pid);
			int killed = kill(pid,SIGINT);
			if(killed != 0)
			{
				perror("Couldn't kill compute processes");
				exit(-1);
			}
		}
	}
	
	sleep(5);
	//detach shared memory
	int success = shmdt(mem);
	if(success != 0)
	{
		perror("Couldn't detach shared memory");
		exit(-1);
	}
	
	//remove shared memory
	success = shmctl(sharedMID, IPC_RMID, 0);
	if(success != 0)
	{
		perror("Couldn't remove shared memory");
		exit(-1);
	}
	
	//remove message queue
	success = msgctl(messageQID, IPC_RMID, NULL);
	if(success != 0)
	{
		perror("Couldn't remove message queue");
		exit(-1);
	}
	
	//exit cleanly
	exit(0);
}

//sort the perfect numbers as they are inserted
int insertNum(int num, int idx)
{
	bool done = false;
	
	for(int i = 0; i < 10; i++)
	{
		if( (num < mem->perfect[i]) && (mem->perfect[i] !=0) )
		{
			for(int j = 9; j > i; j--)
			{
				mem->perfect[j] = mem->perfect[j-1];
			}
			
			mem->perfect[i] = num;
			idx = i;
			done = true;
		}
		if(done) break;
		
	}
	while(!done)
	{
		if(mem->perfect[idx] == 0)
		{
			mem->perfect[idx] = num;
			done = true;
			break;
		}
		idx++;
	}
	
	return idx;
}


int main(int argc, char * argv[])
{
	int processIndex, perfectIndex, pid;
	
	//printf("Size = %d",sizeof(sharedMemory));
	//create shared memory segment
	sharedMID = shmget(SHAREDM_KEY,sizeof(sharedMemory), IPC_CREAT | 0660);
	
	//check if shared memory has already been created
	if(sharedMID == -1)
	{
		perror("Shared memory already exists");
		exit(-1);
	}
	
	//point to beginning of bitmap
	mem = shmat(sharedMID, NULL, 0);
	
	//clear shared memory before we get started
	memset(mem->bitmap, 0, sizeof(mem->bitmap));
	memset(mem->perfect, 0, sizeof(mem->perfect));
	memset(mem->processList, 0, sizeof(mem->processList));

	//get shared message queue
	messageQID = msgget(MESSAGEQ_KEY, IPC_CREAT | 0660);
	
	//check if message queue already exists
	if(messageQID == -1)
	{
		perror("Message queue already exists");
		exit(-1);
	}
	
	//create an empty sigaction
	struct sigaction act = {};
	memset(&act,0,sizeof(act));
	sigemptyset(&act.sa_mask);
    	act.sa_flags = 0;
    	
    	//set up signal handler
	act.sa_handler = apocalypse;
  	
	
	//setup signal handler
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
	
	message msg;
	
	while(1)
	{
		//printf("Waiting for message... \n");
		msgrcv(messageQID,&msg,sizeof(msg.body),-3,0);
		if(msg.type == PERFECTION)
		{
			//printf("PERFECT number is: %d\n",msg.body);
			
			int perfectNum = msg.body;
			int i = 0;
			bool foundBefore = false;
			
			//check to see if we've found this perfect number already
			while(i < perfectIndex)
			{
				if (mem->perfect[i] == perfectNum) {
					foundBefore = true;
					//printf("This perfect number has already been found: %d\n",msg.body);
					break;
				}
				i++;
			}
			
			//update if we have not found this before
			if(!foundBefore)
			{
				perfectIndex = insertNum(perfectNum,perfectIndex);
				perfectIndex = perfectIndex + 1;
			}
		}
		else if(msg.type == GET_INDEX)
		{
			int procID = msg.body;
			int idx;
			
			for(int i = 0; i < 20; i++)
			{
				//printf("Process at index %d is  %d\n",i,mem->processList[i].pid);
				if(mem->processList[i].pid == 0)
				{
					idx = i;
					//update the process map with the correct PID
					mem->processList[idx].pid = procID;
					
					//send the corresponding index within the process map
					msg.type = INDEX_SUCCESS;
					msg.body = idx;
					if(msgsnd(messageQID, &msg, sizeof(msg.body), 0) != 0)
					{
						perror("Failed to send INDEX_SUCCESS message");
						exit(-1);
					}
					break;
				}
				
				//handle the cases where more than 20 processes are attached
				if( (i == 19) && (mem->processList[i].pid != 0) )
				{
					printf("You can only run up to 20 compute processes.\n");
					
					kill(procID,SIGINT);
					break;
				}
				
			}
			
			
			//printf("Process at index %d is  %d\n",idx,mem->processList[idx].pid);

		}	
		else if(msg.type == REQUEST_MPID)
		{
			//set the PID of manage
			int mpid = getpid();
			//printf("Manage PID = %d \n",mpid);
			
			//send the corresponding index within the process map
			msg.type = RECEIVE_MPID;
			msg.body = mpid;
			if(msgsnd(messageQID, &msg, sizeof(msg.body), 0) != 0)
			{
				perror("Failed to send RECEIVE_MPID message");
				exit(-1);
			}
			break;
		}
	}
	
}
	
	
	
	
	
	

