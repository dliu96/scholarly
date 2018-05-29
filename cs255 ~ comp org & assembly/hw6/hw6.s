* len_a is the length of the array 
* if you change the array A for testing purposes, remember to adjust len_a as necessary


        xdef Start,Start2,Stop,End
        xdef A, len_a, key_1 key_2, ans_1, ans_2

        xref BinarySearch

*****************************************************************************
* Main program: call BinarySearch twice to search the array for two different key values
*               stores the results in ans_1 and ans_2 respectively.
*
* DO NOT change the main program.
* Write your BinarySearch routine in the search.s file
*****************************************************************************
Start:
        move.l #A, -(a7)                * Put address of the array on stack frame
        move.l key_1, -(a7)             * Put the key on the stack frame
        move.l #0, -(a7)                * Put the low value on the stack frame
        move.l len_a, -(a7)             * Put the high value ont the stack frame
        jsr BinarySearch                * Search for key in the array
	
	bra Stop
	
        adda.l #16, a7                  * Remove parameter from stack frame
        move.l d0, ans_1                * Set ans_1 to the index of key value in array

Start2:
        nop
        
        move.l #A, -(a7)                * Repeats the process for second key value
        move.l key_2, -(a7)
        move.l #0, -(a7)
        move.l len_a, -(a7)
        jsr BinarySearch

        adda.l #16, a7
        move.l d0, ans_2



* ********************************************************************
* Do not touch the stop label - you need it to stop the program
* ********************************************************************
Stop:   nop
        nop

A:      dc.l 0,2,5,9,12,17,22,24,26,37,42,47,51,55,57,59,62,67,75,88,92,99
len_a:  dc.l 21

key_1:  dc.l 37
key_2:  dc.l 60

ans_1:  ds.l 1
ans_2:  ds.l 1



End:

        end

