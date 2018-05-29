        xdef BinarySearch


BinarySearch:
******************************************************
* Write your recursive binary search assembler subroutine here
*
*
*   private int binarySearch(int[ ] a, int x,
*         int low, int high) {
*      if (low > high) return -1; 
*      int mid = (low + high)/2;
*      if (a[mid] == x) return mid;
*      else if (a[mid] < x)
*         return binarySearch(a, x, mid+1, high);
*      else // last possibility: a[mid] > x
*         return binarySearch(a, x, low, mid-1);
*   }
*
*    BinarySearch: input stack frame (see pdf for details)
*                    
******************************************************

binarySearch:

	move.l	4(a7),high
	move.l	8(a7),low
	move.l	12(a7),x
	move.l	16(a7),a0
	
if1:
	move.l	high,d1
	cmp.l	low,d1
	blt	notFound
	add.l	low,d1
	divs	#2,d1
	move.w	d1,d2
	ext.l	d2
	move.l	d2,mid
	bra	if2

if2:
	move.l	mid,d3
	muls	#4,d3
	move.l	(a0,d3),d3
	move.l	d3,a_mid
	cmp.l	x,d3
	beq	found
	cmp.l	x,d3
	blt	searchUpperHalf
	cmp.l	x,d3
	bra	searchLowerHalf

searchUpperHalf:
	nop
	
	move.l	a0, -(a7)               * Put address of the array on stack frame
        move.l	x, -(a7)		* Put the key on the stack frame
        move.l	mid,d4
        add.l	#1,d4
        move.l	d4, -(a7)               * Put the low value on the stack frame
        move.l	high, -(a7)             * Put the high value ont the stack frame
        bsr	binarySearch

searchLowerHalf:
	nop
	
	move.l	a0, -(a7)               * Put address of the array on stack frame
        move.l	x, -(a7)		* Put the key on the stack frame
        move.l	low, -(a7)              * Put the low value on the stack frame
        move.l	mid,d4
        sub.l	#1,d4
        move.l	d4,-(a7)             * Put the high value ont the stack frame
        bsr	binarySearch
        
found:
	move.l	mid,d0
	bra	Stop
	
	
	
notFound:
	move.l	#-1,d0
	bra	Stop
	


	rts

* *****************************************************************************
* If you need local variables, you can add variable definitions below this line
* *****************************************************************************

x:	ds.l	1
low:	ds.l	1
high:	ds.l	1
mid:	ds.l	1
a_mid	ds.l	1
        end
