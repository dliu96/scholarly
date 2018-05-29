/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Derek Liu
*/


#define EXTERN
#define BITSPERSEG  (8*256*sizeof(int))
#include <stdio.h>
#include <stdlib.h>


typedef struct _seg {  /* definition of new type "seg" */
    int  bits[256];
    struct _seg  *next,*prev;        
}seg  ;

seg* MAPHEAD; //global pointer to head variable
seg* lastSeg;
int lastSegNum;


seg* whichseg (int j) {
        
        seg *segpointer = MAPHEAD;
        int segnum;

        segnum = (( (j + 1)/2 ) - 1)/BITSPERSEG;
        //printf("Located in segment %d.\n",segnum);
	
	if(segnum == 0)
	{
		return segpointer;
	}
	else if (segnum == lastSegNum)
	{
		segpointer = lastSeg;
		return segpointer;
	}
	
	
        for (int i = 0; i < segnum; i++){
                segpointer = segpointer->next;
        }

        //printf("The address where this segment is located at is: %d.\n",segpointer);
	lastSegNum = segnum;
	lastSeg = segpointer;
	
        return segpointer;

}

int whichint(int j){
        
        int segnum,intindex,intnum;

        segnum = (( (j + 1)/2 ) - 1)/BITSPERSEG;

        intindex = j/64;

        intnum = (intindex - (255 * segnum)) - segnum;

        //printf("Located in integer %d.\n",intnum);
        return intnum;

}

int whichbit(int j){
        int bitnum;

        bitnum = (( (j + 1)/2 ) - 1) % BITSPERSEG; /* bit location in segment [0,8191]*/

        bitnum = bitnum  % 32; /* bit location in integer */

        //printf("Located at bit %d.\n",bitnum);
        return bitnum;
}


int clearAll()
{
	seg * pt;
	int i;
	pt = MAPHEAD;
	
	//traverse linked list
	while(pt->next != NULL){
		//set all ints to 0
		for(i = 0; i < 256; i++)
		{
			pt->bits[i] = 0;
		}
		pt = pt->next;
	}
	
	return 1;
}

//for debugging purposes only
void printBits(int num){
    
    int size = 32;
    int maxPow = 1<<(size*8-1);
    int i=0,j;
    
    for(;i<size;++i)
    {
    // print last bit and shift left.
    	printf("%u ",num&maxPow ? 1 : 0);
    	num = num<<1;
    }
}

int marknonprime(int j)
{

	//setBitTo1
	seg * segment;
	int integer, bit;
	
	segment = whichseg(j);
	integer = whichint(j);
	bit = whichbit(j);
	
	//printf("Segment: %d, Int: %d, Bit: %d \n", segment, integer, bit);
	
	//printBits(segment->bits[integer]);
	setBitTo1(segment, integer, bit);
	
	//printBits(segment->bits[integer]);
	
}

int setBitTo1(seg * segment, int integer, int bit)
{

	segment->bits[integer] |= 1 << (31 - bit);
	
	return 1;
}

int testprime(int j) 
{
	seg * segment;
	int integer, bit;
	
	segment = whichseg(j);
	integer = whichint(j);
	bit = whichbit(j);
	
	//printf("Segment: %d, Int: %d, Bit: %d \n", segment, integer, bit);
	//printBits(segment->bits[integer]);	
	
	if (((segment->bits[integer]) & (1<< (31-bit) )) != 0 ){
		return 1;
	}
	else{
		return 0;
	}
	
}

int sieveOfDerek(int n)
{
	//disregard 1
	marknonprime(1);
	
	for(int i = 3 ; i*i <= n; i = i + 2)
	{
		if(testprime(i) == 0)
		{
			for(int j = 3 ; i*j <= n; j = j + 2)
			{
				//printf("i = %d | j = %d | i*j = %d \n",i,j,i*j);
				int product = i*j;
				marknonprime(product);
			} 
		}
	}
	
}

int countprimes(int n)
{
        int i,count;
        i = 3;
        count = 0;

        while (i <= n)
        {
                if(testprime(i) == 0)
                        count = count + 1;
                i = i+2;
        }
        return count;
}

int goldbach(int g)
{
	int start, end;

	start = 3;
        end = g - 3;
	
	//Initialize the starting and ending segment for decomposition 
	seg *startSeg, *endSeg;
	
	startSeg = whichseg(start);
	endSeg = whichseg(end);
	//printf("Start Seg: %d | End Seg: %d \n",startSeg,endSeg);
	
	//Initialize the starting and ending integer for decomposition 
	int startInt, endInt;

	startInt = whichint(start);
	endInt = whichint(end);
	//printf("Start Int: %d | End Int: %d \n",startInt,endInt);

	//Initialize the starting and ending bit for decomposition
	int startBit, endBit;
	
	startBit = whichbit(start);
	endBit = whichbit(end);
	//printf("Start Bit: %d | End Bit: %d \n",startBit,endBit);

	int count, start_solution, end_solution;
	
	count = 0;
	start_solution = 0;
	end_solution = 0;
	
	while(1){
		
		//check boundaries of bit within integer MIGHT NOT NEED THIS!!
		if(endBit < 0)
		{
			endBit = 31;
			endInt = endInt -1;
			
			if(endInt < 0){
				endInt = 255;
				endSeg = endSeg->prev;
			}
		}
		if (startBit > 31)
		{
			startBit = 0;
			startInt = startInt + 1;
			
			if(startInt>255)
			{
				startInt = 0;
				startSeg = startSeg->next;
			}
		}
		
		if( !((startSeg->bits[startInt])&(1<<(31-startBit))) && !((endSeg->bits[endInt])&(1<<(31-endBit)))){
	
			count++;
			start_solution = start;
			end_solution = end;
			//printf("%d + %d = %d\n",start_solution,end_solution,g);
		}
		
		start += 2;
		end -= 2;
		startBit ++;
		endBit --;
		
		if(start>(g/2)) break;
	}
	
	printf("The final decomposition is: %d + %d = %d\n",start_solution,end_solution,g);	
	printf("There are %d different solutions.\n", count);
	
	return 1;	
}

int main(int argc, char *argv[]) 
{

        seg *head,*pt,*temp;
        int      i;
        int howmany;
        int n;
        if (argc == 2) sscanf(argv[1],"%d",&howmany);
                else scanf("%d",&howmany);

        n = howmany;

        howmany = ( ((howmany + 1)/2) + BITSPERSEG -1)/BITSPERSEG;

        head= ( seg * ) malloc(sizeof(seg));
        MAPHEAD = head; 
        pt = head;
        for (i=1;i<howmany;i++) {
                temp = pt;
                pt->next = (  seg *) malloc(sizeof (seg)); 
                pt=pt->next;
		
		//Creating backward links
                if(pt != head){
                        pt->prev=temp;   
                }
                
        }
	
	//Set all bits to 0
	clearAll();
	
        printf("Done allocating %d nodes\n",i);

	//Set up bitmap for all odd primes
        sieveOfDerek(n);
        
	//Debugging
        printf("\n");        
        //printBits(head->bits[0]);
	
	//Check total number of primes within N to validate sieve.
        int numberOfPrimes = countprimes(n);
        printf("\nThe number of odd primes less than or equal to %d is:  %d.\n", n,numberOfPrimes);
	
	//Perform goldbach decomposition.
	int g;
	printf("Time for Goldbach's decomposition! Please enter an even number greater than 5 and less than the previously specified N: \n");
	
	while((scanf("%d", &g) > 0)){
		
		if(g % 2){
			printf("Sorry, please enter an even number. \n");
			continue;
		}
		else if (g > n){
			printf("Sorry, please enter a number less than the previously specified N. \n");
			continue;
		}
		
		goldbach(g);	
	}

} 
