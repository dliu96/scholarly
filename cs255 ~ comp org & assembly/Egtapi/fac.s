* File: 255/demo/fac.s
* ------- The factorial recursive subprogram
*
*
        xdef Start,Stop,End
	xdef fac,n,result

* ===============================================
* main: result = fac(4)
* ===============================================
Start:  
	movea.l  #12345, a6

	move.l n,-(a7)      
        bsr    fac           ; fac(4)

        adda.l  #4,a7        ; pop parameter from stack
        move.l d0,result     ; Put 4! in result

Stop:   nop

n:      dc.l 3
result: ds.l 1


*
* ===============================================
* int fac(int n)
* {  
*    if (n == 0)
*       return(1);
*    else
*    { 
*	return (n * fac(n-1));
*    }
* }
*    Input: n on stack
*    Output: n! in register d0
* ===============================================
*
fac:
********************************* PRELUDE
        move.l a6,-(a7)    ; Save caller's frame pointer
        move.l a7,a6       ; Setup my own frame pointer
        suba.l #0,a7       ; No local variables (you can omit this instruction)
*********************************
* ----------------------------- ; Testing n == 0....
        move.l 8(a6),d0    ; 
        cmp.l  #0,d0       ; n == 0 ??
        bne    Else        ;

        move.l #1,d0       ; then part: return 1 in D0

********************************* POSTLUDE
        move.l a6,a7       ; Deallocate local variables
        move.l (a7)+,a6    ; restore caller's frame pointer
*********************************
        rts

Else:
        move.l 8(a6),d0    ; 
        sub.l #1,d0        ; D0 = n - 1
*
* ----------------------------- ; fac is calling fac now !!!!
*
        move.l d0,-(a7)    ; Pass (n-1) as parameter to fac
        bsr    fac         ; Call fac using (n-1) as parameter (i.e.: fac(n-1)
        adda.l #4,a7       ; Clean up parameter from stack when fac returns

*			   ; Note: d0 = output of fac(n-1) !!!

        move.l 8(a6),d1    ; d1 = n
        muls   d1,d0       ; d0 = n*fac(n-1)

*                          ; Now we are ready to exit... Watch the stack !

********************************* "POSTLUDE" code
        move.l a6,a7       ; Deallocate local variables
        move.l (a7)+,a6    ; restore caller's frame pointer
*********************************
        rts


End:

        end

Use this .gdbinit and single step to see the stack changing:
b start
b stop
disp/28dw 131060-100
disp/x $sp
disp/x $fp
disp/d $d0
disp/dw &result
disp/i $pc

