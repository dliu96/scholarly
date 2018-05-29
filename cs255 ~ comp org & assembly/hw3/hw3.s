* Derek Liu
* dliu55@emory.edu
* This code was my own work. It was written without consulting code written by other students. Derek Liu
*
	xdef Start, Stop, End
	xdef Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10
	xdef A, B, C
	xdef i, j, k
	xdef head
	xdef ans_b, ans_w, ans_l

Start:
*******************************************************************
* Put your assembler instructions here
* Write the answer to each question after the corresponding label.
* DO NOT REMOVE ANY LABEL IN THIS ASSIGNMENT
* *** Failure to do so will result in point dedections !!! ***
*******************************************************************

Q1: move.l	#A, a0
    move.b	4(a0),ans_b
*         ans_b = A[4];





Q2: move.l	#B,a1
    move.w	12(a1),d0
    ext.l	d0
    move.l	d0,ans_l
*         ans_l = B[6];





Q3: move.l	#C,a2
    move.l	k,d0
    muls	#4,d0
    add.l	d0,a2
    move.l	0(a2),ans_l
*         ans_l = C[k];





Q4: move.l	#A,a0
    move.l	k,d0
    move.b	i,d1
    ext.l	d1
    add.l	d1,d0
    add.l	d0,a0
    move.b	0(a0),d1
    ext.w	d1
    move.w	d1,ans_w
*         ans_w = A[i + k];      




Q5: move.l	#C,a2
    move.b	i,d0
    ext.l	d0
    move.w	j,d1
    ext.l	d1
    add.l	d0,d1
    muls	#4,d1
    add.l	d1,a2
    move.l	0(a2),d3
    move.w	d3,ans_w
*         ans_w = C[i + j];      




Q6: move.l	#A,a0
    move.l	#B,a1
    move.l	k,d0
    move.b	i,d1
    ext.l	d1
    muls	#2,d1
    add.l	d0,a0
    add.l	d1,a1
    move.b	0(a0),d2
    move.w	0(a1),d3
    add.l	d2,d3
    move.l	d3,ans_l
*         ans_l = A[k] + B[i];  




Q7: move.l	#A,a0
    move.w	j,d0
    ext.l	d0
    add.l	d0,a0
    move.b	0(a0),d1
    add.b	#64,d1
    ext.l	d1
    move.l	#A,a1
    add.l	d1,a1
    move.b	0(a1),d2
    move.l	d2,ans_l
*         ans_l = A[A[j] + 64];      



	
Q8: move.l	#B,a1
    move.w	26(a1),d0
    move.w	d0,ans_w
*         ans_w = B[13]





Q9: move.l	head,a0
    move.w	4(a0),d4
    ext.l	d4
    move.l	d4,ans_l
*	  ans_l = head.value2;



Q10: move.l	head,a0
     move.l	6(a0),a0
     move.l	(a0),d5
     move.w	d5,ans_w
*	  ans_w = head.next.value1;



*************************************************
* Don't write any code below this line
*************************************************

Stop:	nop
	nop

*************************************************
* Don't touch these variables
* You do NOT need to define more variables !!!
*************************************************

************************************************************************
* Note the use of the even directive to locate the variables ans_w and j
* at an EVEN address due to the variables ans_b and i being bytes
* Short and int variables MUST start on an even address (or you 
* will get an "odd address" error)
************************************************************************

ans_b: ds.b 1
	even
ans_w: ds.w 1
ans_l: ds.l 1

i:     dc.b  3
	even
j:   dc.w  4
k:   dc.l  5

A:  dc.b   -11, 22, -33, 44, -55, 66, -77, 88, -99, 123

B:  dc.w   111, -222, 333, -444, 555, -666, 777, -888, 999, -5171

C:  dc.l   1111, -2222, 3333, -4444, 5555, -6666, 7777, -8888, 9999, -9183


head:   dc.l  list1

list3:  dc.l 2468
        dc.w 88
	dc.l list4
list2:  dc.l 1470
        dc.w 78
	dc.l list3
list4:  dc.l 4567
        dc.w 65
	dc.l list5
list1:  dc.l 1357
        dc.w 98
	dc.l list2
list5:  dc.l 9876
        dc.w 54
	dc.l 0


End:
	end

