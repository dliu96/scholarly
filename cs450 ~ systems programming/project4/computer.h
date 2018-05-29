/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu
*/

#define MESSAGEQ_KEY 	30352
#define SHAREDM_KEY 	50425

#define GET_INDEX 	1
#define PERFECTION 	2
#define REQUEST_MPID	3
#define INDEX_SUCCESS 	4
#define RECEIVE_MPID	5


typedef struct {
	int pid;
	int numFound;
	int numTested;
	int numSkipped;
} procSummary;

typedef struct {
	int bitmap[1048576];
	int perfect[20];
	procSummary processList[20];
} sharedMemory;

typedef struct {
	long int type;
	int body;
} message;


