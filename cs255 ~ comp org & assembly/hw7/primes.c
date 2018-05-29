#define EXTERN
#define MAX 1000000


#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void setBitTo1(int n)
{
 	//Set bit related number to 1 meaning prime
 	prime[(n/32)] |=  1 << (n%32);
 
}


void setAll()
{	
	//Set all numbers from 0 to 32,000,000 to 1 as possible prime candidates
 	int i;
 	for(i = 0; i < (32*MAX); i++){
		setBitTo1(i);
  	}
}




void clearBit(int n)
{
	//Set number to in bit array to 0. This means it is no longer a prime possibility 
 	prime[(n/32)] &= ~(1 << (n%32));

}

int testBitIs1(int n)
{
	//Check the true/false (1/0) of a number 'n'
	if ((prime[n/32] & (1 << (n%32) ))  ){
		return 1; //not prime
	}
	return 0; //prime!
}

void sieveOfE(int n)
{
	setAll();    //all numbers are possible primes
	
	clearBit(0); // 0 is not prime
	clearBit(1); // 1 is not prime
	
	int i,j,k;
	
	k = 2; //start at 2 as first prime
	
	while (k <= n)
	{
		for(i=k; i<= n; i++)
			if(testBitIs1(i) == 1) //prime number is found, break from for loop
				break;
		for(j=2*i; j<=n; j = j + i)
			clearBit(j); //set all multiples of i to not prime.
		
		k = i+1; //increment k
		
	}			
}

int countPrimes(int n)
{
	int i,count;
	i = 2;
	count = 0;
	
	//count the number of 1's that indicate primes
	while (i <= n)
	{
		if(testBitIs1(i))
			count = count + 1;
		i++;
	}
	return count;
}

void printPrimes(int n)
{
	int i;
	i = 2;
	
	//print all the numbers correlated with the bit as 1
	while (i <= n)
	{
		if(testBitIs1(i))
			printf("%d \n",i);
		i++;
	}
	
}
  


