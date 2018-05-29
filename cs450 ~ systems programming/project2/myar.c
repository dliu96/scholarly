/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu

Also attempting the v option!!!
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ar.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <utime.h>
#include <time.h>
#include <dirent.h> 

//extern int errno ;
off_t HEADER_OFFSET;


void find_file_name(struct ar_hdr *header)
{
	int i = 0;
        
        while(i < 15)
        {
        	//printf("%d ", i);
        	//printf("%d \n", (int)header->ar_name[i] );
            
            	//end name if / or Null byte is encountered
            	if((int)header->ar_name[i] == 32 || (int)header->ar_name[i] == 47 )
		{
			header->ar_name[i] = '\0';
			break;
		}
		    i++;
        }
}

void format_mode(char * mode)
{
	int j;			
	for (j=3;j<6;j++)
	{
		int pos = mode[j] - '0';
		int posx = pos % 2;
		pos = pos/2;
		int posw = pos % 2;
		pos = pos/2;
		int posr = pos % 2;

		if (posr)
		{
			printf("r"); 
		} else 
		{
			printf("-");
		}
		if (posw)
		{
			printf("w");
		} else 
		{
			printf("-");
		}
		if (posx)
		{
		 printf("x"); 
		} else 
		{
			printf("-");
		}
	}
}

void t(int fd, bool v)
{
	//variable declarations
    char headerName[15];
    struct ar_hdr *header;
    int hdr_length = sizeof(struct ar_hdr);
    int size;
    
    //allocate memory for the archive header
    header = malloc(hdr_length);
    //printf("fd: %d \n", fd);
    
    while (read(fd, header, sizeof(struct ar_hdr)) == sizeof(struct ar_hdr))
    {
        
        find_file_name(header);
        size = (int)strtol(header->ar_size,NULL,10);
        
        if(v){
		int uid, gid;
		time_t timestamp;
		char time[20] = "";

		format_mode(header->ar_mode);
		uid = atoi(header->ar_uid);
		gid = atoi(header->ar_gid);
		printf("%6d/%-6d ",uid, gid);
		printf("%d", size);
			 
		timestamp = strtol(header->ar_date,NULL,10);
		
		strftime(time, 20, " %b %e %R %Y", localtime(&timestamp));
		printf("%-20s", time);
		printf("%-16s\n",header->ar_name);
        } else
        {
        	printf("%s\n",header -> ar_name);
        }

        
        if(size%2)
        {	
        	size++;
        	//printf("New size: %d \n", size);
        }
        
        lseek(fd,size,SEEK_CUR);
            
            
    }
    
    close(fd);
        



}

void q(int ar_fd, char *f_name)
{

	int f_block_size;

	int f_fd = open(f_name, O_RDONLY|O_CREAT,0666);

	struct ar_hdr *header_info = malloc(sizeof(struct ar_hdr));
	struct stat *f_info = malloc(sizeof(struct stat));


	//printf("file: %s \n",f_name);

	//get file status and size
	fstat(f_fd,f_info);
	f_block_size = (int)f_info->st_size;

	//copy file contents over to a temporary ar_hdr struct
	snprintf(header_info->ar_name, 16, "%-16s", f_name);
	snprintf(header_info->ar_date, 12, "%-12ld", f_info->st_mtime);
	snprintf(header_info->ar_uid, 6, "%-6llu",(long long unsigned int) f_info->st_uid);
	snprintf(header_info->ar_gid, 6, "%-6llu", (long long unsigned int) f_info->st_gid);
	snprintf(header_info->ar_mode, 8, "%-8o", f_info->st_mode);
	snprintf(header_info->ar_size, 10, "%-10llu",(long long unsigned int) f_info->st_size);
	memcpy(header_info->ar_fmag, ARFMAG, 2);

	//printf("f_blocksize = %d \n", f_block_size);
	//printf("header_info = %s \n", header_info->ar_date);
	//printf("header_info = %s \n", header_info->ar_size);

	//find the end of the archive and adjust to start new header on an even byte
	if ((lseek(ar_fd, 0, SEEK_END) % 2) == 1)
	{
		write(ar_fd, "\0", 1);
	}
	
	lseek(ar_fd, 0, SEEK_END);
	write(ar_fd, (char *) header_info, sizeof(struct ar_hdr));

	char * f_content[f_block_size];
	//printf("%d \n", f_block_size);
	//printf("%zd \n", sizeof(f_content));

	//write the body of the file to the archive
	int j =0;
	while( (j = read(f_fd,f_content,f_block_size)) )
	{
			
		write(ar_fd,f_content,j);

	}
	
	close(f_fd);

}


bool search_archive(int ar_fd,struct ar_hdr * f_header,char * f_name, int f_size)
{
	bool found = 0;

	while (read(ar_fd, f_header, sizeof(struct ar_hdr)) == sizeof(struct ar_hdr)){
		
		find_file_name(f_header);
		f_size = (int)strtol(f_header->ar_size,NULL,10);
		
		if ((strcmp(f_header->ar_name, f_name) != 0))
		{
			//skip
			if (f_size%2) 
			{
				f_size++;
			}

			lseek(ar_fd,f_size,SEEK_CUR);
			printf("... %s\n",f_header->ar_name);
	
		} 
		else
		{
			//break
			HEADER_OFFSET = lseek(ar_fd, 0, SEEK_CUR);
			found = 1;
			printf("Found %s!\n",f_header->ar_name);
			break;
		}
	}

	return(found);

}

void x(int ar_fd, char *f_name)
{
	int new_fd, f_blocks, f_size;
	struct ar_hdr * f_header = malloc(sizeof(struct ar_hdr));
	struct utimbuf *f_utime = malloc(sizeof(struct utimbuf));
	struct stat *f_stat = malloc(sizeof(struct stat));
   	
   	mode_t f_mode;
    time_t f_time;

    off_t cur,file_start;
	ssize_t num_read;
	int read_size;

	//printf("name: %s\n",f_name);
	
	lseek(ar_fd,8,SEEK_SET);
	
	if(search_archive(ar_fd, f_header, f_name, f_size))
	{
		f_mode = (mode_t) strtol(f_header->ar_mode, NULL, 8);
		//printf("1: %d \n", f_mode);
		//printf("2: %d \n", f_header->ar_mode);

		//printf("name: %s \n", f_header->ar_name);

		new_fd = open(f_header->ar_name,(O_WRONLY | O_CREAT), 0666);

		if(new_fd < 0)
		{
			printf("Error creating new file. \n");
			//int errnum = errno;
  			//fprintf(stderr, "Value of errno: %d\n", errno);
		}

		file_start = HEADER_OFFSET;

		if (file_start < 0)
		{
			printf("Error finding file. \n");
		}
		

		//copy content from archive 
		cur = lseek(ar_fd, file_start, SEEK_SET);
		read_size = (int) (atoll(f_header->ar_size));

		//printf("%d\n", read_size);
		char content[read_size];

		//printf("file_start = %jd\n", (intmax_t) file_start);
		//printf("cur = %jd\n", (intmax_t)cur);


		while ((num_read = read(ar_fd, content, read_size)) > 0) {
			
			//printf("Yay!: %s \n", content);
			

			write(new_fd, content, num_read);
			cur += num_read;

		}
		
		//write(new_fd,f_content,actual_size);

		
		f_time = (time_t)strtol(f_header->ar_date,NULL,10);
		f_utime->modtime = f_time;
		f_utime->actime = f_time;

		//change time ?
		utime(f_header->ar_name,f_utime);
		
		close(new_fd);
		
		
	
	}	
	else
	{
		printf("Sorry, I couldn't find what you were looking for. Please try again!");
	}
	
}

int count_ordinary_files()
{
	DIR *dirp;  
    struct dirent *files;
    int count=0;

    //open current directory and count the regular files
    dirp = opendir("./");
    while(files = readdir(dirp)) 
        { 
            if(files->d_type == DT_REG)
            { 
            	//printf("file: %s \n",files->d_name);
                count++;
            }       
        } 
    //printf("# of ordinary files: %d \n",count);
    return count;
}

void A(int ar_fd, char * ar_name)
{
	DIR * dirp;
	struct dirent *files;
	int n = count_ordinary_files();
	char file_list [n][50];
	int i = 0;

	time_t current_time = time(0);
	
	//printf("n = %d \n",n);
	//populate list of ordinary files
	dirp = opendir("./");
	if (dirp)
	{
    	while ( ((files = readdir(dirp)) != NULL) && (i < n))
    	{
    		if((files->d_type == DT_REG )&& (strcmp(files->d_name,ar_name) != 0))
            { 
                strcpy(file_list[i], files->d_name);
                //printf("%s\n", files->d_name);
                i++;
            } 
      		
    	}

    	closedir(dirp);
  	}
  	
  	//loop through ordinary files and append all that have been 
  	//modified within last two hours
  	int j = 0;
  	
  	while(j<n){
  	
  		//printf("%s\n",file_list[j]);
  		char * f_name = file_list[j];
		//printf("ar = %s\n",ar_name);
  		struct stat *f_stat = malloc(sizeof(struct stat));
		stat(f_name,f_stat);
	
			
		if((difftime(time(0), f_stat->st_mtime) <= 7200) )
		{
			//printf("%s\n",f_name);
			//printf("%f\n",difftime(time(0), f_stat->st_mtime));
			q(ar_fd, f_name);
			
		}
		j++;

  	}

}



//T,Q,X,A

int main(int argc, char *argv[])
{
	char arch[8];
    int fd;
    char *key, *ar_name, *f_name;
    bool v;

    if(argc < 3)
    {
            printf("Not enough arguments!");
    }

    key = argv[1];
    ar_name = argv[2];
    f_name = argv[3];
    
    if(argc > 4)
    {
    	printf("Sorry, I can only handle one file at a time right now! Please try again.");
    }




    if (*key == 'q' || *key == 'A' )
    {
    	//create a new archive file if it doesn't already exist
    	fd = open(ar_name, O_RDWR | O_CREAT, 0666);
    	printf("Opening archive file: %s \n", ar_name);
    	write(fd,ARMAG, 8);
    }
    else
    {
    	//try to open the archive
    	fd = open(ar_name, O_RDWR);

    	//error handler
    	if(fd == -1)
    	{
    		printf("The archive you specified does not exist. Please try again. \n");
    	}

    	//get the magic number
    	read(fd,arch,8);
    }
    

    if(*key == 'q')
    {
    	q(fd,f_name);

    } 
    else if(*key == 'x')
    {
    	x(fd,f_name);

    } 
    else if(*key == 't')
    {    
        
        //print a concise table of contents of the archive
    	t(fd,0);

    } 
    else if(*key == 'v')
    {
   
        t(fd,1);
    } 
    else if(*key == 'd')
    {
    	printf("Sorry, I do not understand the option you have specified.\n"); 

    } 
    else if(*key == 'A')
    {
        A(fd,ar_name);
    }
    else
    {
    	printf("Sorry, I do not understand the option you have specified.\n");
    }

    close(fd);

}
