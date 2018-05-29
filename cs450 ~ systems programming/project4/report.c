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

int messageQID, sharedMID,procIndex, mpid;
sharedMemory *mem;

int main(int argc, char * argv[])
{
	if(argc > 2)
	{
		printf("USAGE: ./report [ -k (optional)]");
		exit(-1);
	}
	else if(argc == 2)
	{
		//check if -k is the second argument
		if( (strcmp(argv[1], "-k") == 0) || (strcmp(argv[1], "k") == 0) )
		{
			//attach to message queue
			messageQID = msgget(MESSAGEQ_KEY,0);
			if(messageQID == -1)
			{
				perror("Unable to find message queue");
				exit(-1);
			}
	
			message msg;
	
			//send the corresponding index within the process map
			msg.type = REQUEST_MPID;
			msg.body = 0;
			if(msgsnd(messageQID, &msg, sizeof(msg.body), 0) != 0)
			{
				perror("Failed to send REQUEST_MPID message");
				exit(-1);
			}
	
			//wait for mpid
			msgrcv(messageQID,&msg,sizeof(msg.body),RECEIVE_MPID,0);
	
			mpid = msg.body;
			
			int killed = kill(mpid, SIGINT);
			if(killed != 0)
			{
				perror("Couldn't kill manage process (from report)");
				exit(-1);
			}
		}
		else
		{
			printf("USAGE: ./report [ -k (optional)]");
			exit(-1);
		}
	}
	
	//attach to shared memory segment
	sharedMID = shmget(SHAREDM_KEY,sizeof(sharedMemory), 0);
	if(sharedMID == -1)
	{
		perror("Unable to find shared memory");
		exit(-1);
	}
	
	mem = shmat(sharedMID, NULL,0);
	printf("		 __________\n");
	printf("		 | REPORT |\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
	
	//print process statistics
	int i;
	while(mem->processList[i].pid != 0)
	{
		printf("	Process ID: %d [ index = %d ]\n", mem->processList[i].pid, i);
		printf("	Perfect numbers found: %d \n", mem->processList[i].numFound);
		printf("	Total numbers tested: %d \n", mem->processList[i].numTested);
		printf("	Total numbers skipped: %d \n", mem->processList[i].numSkipped);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
		
		i++;
	}
	
	if(i == 1)
	{
		printf("	| THERE IS %d PROCESS RUNNING |\n", i);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
	}
	else
	{
		printf("      | THERE ARE %d PROCESSES RUNNING |\n", i);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
	}
	
	//print perfect numbers
	int j,cnt;
	printf("	     | PERFECT NUMBERS |\n");
	printf("    ");
	for(j=0;j<20;j++)
	{
		int n = mem->perfect[j];
		if(n != 0)
		{	
			printf("%d ", n);
			cnt++;	
		}
	}
	printf("\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
	printf("	 | TOTAL PERFECT NUMBERS: %d |\n",cnt);
	printf("+++++++++++++++++++++++++++++++++++++++++++++++ \n");
	

}
