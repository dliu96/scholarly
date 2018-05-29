* Derek Liu
* dliu55@emory.edu
* This code was my own work. It was written without consulting code written by other students. Derek Liu
*
* ********************************************************************
* Do not touch the following 2 xdef lists:
* ********************************************************************
        xdef Start, Stop, End
        xdef A, B, len_a, len_b, max, min, sum, common

* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* You can add more xdef here to export labels to emacsim
* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	xdef I, J
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*      Put your assembler program here - between the start and stop label
*      DO NOT define any variables here - see the variable section below
*
*a = [1,2,3,4,5]
*b = [6,7,8,9,10]
*
*min = a[0]
*max = a[0]
*common = 0
*sum = 0
*i = 0
*
*
*while (i < len(a)):
*    j = 0
*   
*    if (a[i] < min):
*        min = a[i]
*    if (a[i] > max):
*        max = a[i]
*
*    sum += a[i]
*
*    while (j < len(b)):
*
*        if (b[j] < min):
*            min = b[j]
*        if (b[j] > max):
*            max = b[j]
*        if (b[j] == a[i]):
*            common += 1
*        if (i == 0):
*            sum += b[j]
*        j = j +1
*    i = i +1
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Start:
	move.l	#A,a0
	
	move.l	(a0),min
	move.l	(a0),max
	move.l	#0,common
	move.l	#0,sum
	
WhileCondition1:
	move.l	len_a,d0
	cmp.l	I,d0
	bgt	While1
	bra	Stop
	
While1:
	move.l	#0,J

	move.l	#A,a0
	move.l	I,d0
	muls	#4,d0
	add.l	d0,a0
	move.l	(a0),d1
	
	move.l	sum,d2
	add.l	d1,d2
	move.l	d2,sum
	bra	ifCondition1
	
	
	
ifCondition1:
	cmp.l	min,d1
	blt	ifBody1
	bra	ifCondition2
	
ifBody1:
	move.l	d1,min
	
ifCondition2:
	cmp.l	max,d1
	bgt	ifBody2
	bra	While2
	
ifBody2:
	move.l	d1,max
	
WhileCondition2:
	move.l	len_b,d0
	cmp.l	J,d0
	bgt	While2
	bra	loopIncrement1

While2:
	move.l	#B,a1
	move.l	J,d0
	muls	#4,d0
	add.l	d0,a1
	move.l	(a1),d3

ifCondition3:
	cmp.l	min,d3
	blt	ifBody3
	bra	ifCondition4
	
ifBody3:
	move.l	d3,min
	
ifCondition4:
	cmp.l	max,d3
	bgt	ifBody4
	bra	ifCondition5
	
ifBody4:
	move.l	d3,max

ifCondition5:
	cmp.l	d3,d1
	beq	ifBody5
	bra	ifCondition6
	
ifBody5:
	move.l	common,d7
	add.l	#1,d7
	move.l	d7,common
	
ifCondition6:
	move.l	I,d0
	cmp.l	#0,d0
	beq	ifBody6
	bra	loopIncrement2
	
ifBody6:
	move.l	sum,d2
	add.l	d3,d2
	move.l	d2,sum
	bra	loopIncrement2

loopIncrement1:
	move.l	I,d5
	add.l	#1,d5
	move.l	d5,I
	bra	WhileCondition1

loopIncrement2:
	move.l	J,d5
	add.l	#1,d5
	move.l	d5,J
	bra	WhileCondition2

	








* ********************************************************************
* Do not touch the stop label - you need it to stop the program
*********************************************************************
Stop:   nop



* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*    Variable Section -   Put your variables here IF you need more
*
*    DO NOT define A, B, len_a, len_b, max, min, sum and common !!!
*    They are already defined below
*
*    You can add more variables below this line if you need them
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

I:	dc.l  0
J:	dc.l  0




* ********************************************************************
* Adjust the arrays (and lengths) below to test different arrays
* ********************************************************************
A:      dc.l  1,2,9,17,11,16,10
B:      dc.l  4,5,22,2,1
len_a:  dc.l  7
len_b:  dc.l  5


* ********************************************************************
* Do not touch anything below this line !!!
* ********************************************************************
max:    ds.l  1
min:    ds.l  1
sum:    ds.l  1
common: ds.l  1

End:    nop
        end
