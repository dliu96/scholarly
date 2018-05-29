* Derek Liu
* dliu55@emory.edu
* This code was my own work. It was written without consulting code written by other students. Derek Liu

        xdef BubbleSort


BubbleSort:
******************************************************
* Write your bubble sort assembler subroutine here
*
*    BubbleSort: input  D0 = addres of int array
*                       D1 = N
*// How the variables are used :*//*// input A: array of integers that needs to be sorted*// n: size of array A*//*// done : integer ( acting as a boolean : 0 = false , 1 = true )*// help : integer ( help variable for swapping elements )*
*void BubbleSort (int [] A, int n){*	done = 0; // 0 represents false*	k = 1;*	while ( done == 0) {*		done = 1; // 1 represents true .*		for (i=0; i < n-k; i ++){*			if (A[i] > A[i +1]){*				help = A[i];*				A[i] = A[i +1];*				A[i+1] = help ;*				done = 0; // Not sorted ...*			}*		}*		k = k + 1;*	}*}
*
******************************************************
	move.l	d0,a0
	move.l	d1,n
	
	move.l	#0,done
	move.l	#1,k
	
whileCondition:
	move.l	done,d5
	cmp.l	#0,d5
	beq	while
	rts
	
while:
	move.l	#1,done
	move.l	#0,i
	
forCondition:
	
	move.l	i,d1
	move.l	n,d2
	move.l	k,d3
	sub.l	d3,d2
	cmp.l	d2,d1
	blt	for
	add.l	#1,d3
	move.l	d3,k
	bra	whileCondition

for:
	bra	ifCondition
	
ifCondition:
	move.l	d0,a0
	move.l	i,d1
	muls	#4,d1
	add.l	d1,a0
	move.l	(a0),d2
	add.l	#4,d1
	move.l	d0,a0
	add.l	d1,a0
	move.l	(a0),d3
	cmp.l	d3,d2
	bgt	if
	move.l	i,d1
	add.l	#1,d1
	move.l	d1,i
	bra	forCondition
	
if:
	move.l	d0,a0
	move.l	i,d1
	muls	#4,d1
	add.l	d1,a0
	move.l	d2,help
	move.l	d3,(a0)
	add.l	#4,a0
	move.l	help,(a0)
	move.l	#0,done
	bra	forCondition
	
	
	



	

* *****************************************************************************
* If you need local variables, you can add variable definitions below this line
* *****************************************************************************
n:	ds.l	1
help:	ds.l	1

done:	dc.l	0
k: 	dc.l	1
i:	dc.l	0


        end
