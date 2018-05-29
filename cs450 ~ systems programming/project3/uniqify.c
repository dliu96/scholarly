/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc, char * argv[])
{
	int sortPipe[2],suppressPipe[2], pid[2];
	int code;
	
	if(pipe(sortPipe) == -1) //first pipe
	{
		perror("pipe");
		exit(-1);
	}
	
	
	if(pipe(suppressPipe) == -1) //second pipe
	{
		perror("pipe");
		exit(-1);
	}
	
	pid[0] = fork(); //first child (sort)
	pid[1] = fork(); //second child (suppress)
	
	
	if(pid[0] == 0) //sort process
	{	
		//redirect the pipes
		dup2(sortPipe[0],0);
		dup2(suppressPipe[1],1);
		
		//close all unused pipes
		close(sortPipe[1]);
		close(sortPipe[0]);
		close(suppressPipe[1]);
		close(suppressPipe[0]);
		
		//execute /bin/sort
		execl("/usr/bin/sort","/usr/bin/sort",NULL);
		fprintf(stderr, "Failed to execute 'bin/sort'\n");
		exit(-1);
		
	}
	else if(pid[1] == 0) //suppress process
	{
		//redirect the pipes
		dup2(suppressPipe[0],0);
		
		//close all unused pipes
		close(suppressPipe[1]);
		close(sortPipe[1]);
		close(sortPipe[0]);
		
		
		char lastWord[37];
		char currWord[37];
		int count;
		bool first = true;
		
		//keep track of last and current words as they come out of the pipe sorted
  		while(fgets(currWord, 37, stdin) != NULL)
  		{
  			//remove the \n
  			currWord[strlen(currWord) - 1] = 0;
 			
  			if(strcmp(lastWord,currWord) == 0) //duplicate words
  			{
  				count++;				
  			} else //new word
  			{
  				//handle the first iteration where lastWord is something weird...
  				if(first) 
  				{
  					printf("		--UNIQIFY--\n");
  					printf("      ~~Written by D. Liu (11/18/17)~~\n");
  					first = false;
  					count++;
  				}
  				else{
  				
  				//formatted print to stdout
  				printf("%-37s %5d\n",lastWord,count);
  				count = 1; //reset count
  				}
  			}
  			//store the current word as the last word for next iteration
  			strcpy(lastWord,currWord);
  		}
  		//print the LAST last word :p
  		printf("%-37s %5d\n",lastWord,count);
  			
  		close(suppressPipe[0]);
	}
	else
	{
		//close all unused pipes
		close(sortPipe[0]);
		close(suppressPipe[0]);
		close(suppressPipe[1]);
		
		//open up write file into the first pipe
		FILE * parsedFile = fdopen(sortPipe[1], "w");
		
		char c;
		char  word[37];
		int i = 0;

		while((c = getchar()) != EOF)
		{
			if(isalpha(c) != 0) //only get alphabetic characters
			{
				
				if(i < 35) //set 35 letter limit
				{
					word[i] = tolower(c); //get lowercase
					i++;
				}
			}
			else //end of word reached
			{
				strcat(word,"\n"); // /bin/sort only takes in new line delimited words!
				
				if(strlen(word) > 5) //discard all words under 5 characters long
				{
					fputs(word,parsedFile);
				}
			
				i = 0; //reset letter count
				memset(word, 0, sizeof(word)); //clear string		
			}
		}
		
		fclose(parsedFile);
		close(sortPipe[1]);
		
		//wait and reap both child processes
		wait(&code);
		wait(&code);
	}
	return 0;
}

