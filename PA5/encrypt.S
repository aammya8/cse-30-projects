//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     

    //////////////////////////////////////////////////////
    // int encrypt(char *IOBUF, char *BOOKBUF, int cnt) //
    //                                                  //
    // encrypts IOBUF with BOOKBUF; updating IOBUF      //
    //////////////////////////////////////////////////////

    .global encrypt
    .type   encrypt, %function
    .equ    FP_OFF, 12

    ///////////////////////////////////////////////
    // scratch and preserved register use:       //
    // r3 temp for nibble swap; hold BOOKBUF[i]  //
    // r4 is loop counter, i, of do-while loop   //
    // r5 holds IOBUF[i]                         //
    ///////////////////////////////////////////////

encrypt:
    // function prologue
    push    {r4, r5, fp, lr}    // stack frame register save
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    mov     r4, 0               // initialize loop counter to 0
.Ldo:
    
    ldrb    r5, [r0, r4]        // read IOBUF[i] into r5

    // swap nibbles, using r3 as temp
    lsl     r3, r5, 28          // move bits 0-3 of IOBUF[i] to leftmost of r3
    lsr     r3, r3, 24          // move bits 0-3 of IOBUF[i] to rightmost of r3
    lsr     r5, r5, 4           // move bits 4-7 of IOBUF[i] to bits 0-3
    orr     r5, r5, r3          // combine swapped nibbles 

    ldrb    r3, [r1, r4]        // read BOOKBUF[i] into r3

    eor     r5, r5, r3          // encrypt by r3 EOR r5

    strb    r5, [r0, r4]        // store encrypted IOBUF[i] into IOBUF[i]

    add     r4, r4, 1           // increment loop counter
    cmp     r4, r2              // loop guard, are all cnt chars encrypted?
    blt     .Ldo                // if not, encrypt next char in IOBUF

    mov     r0, r2              // set return value of encrypt to cnt


.Ldone:
    // function prologue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4, r5, fp, lr}    // remove frame from stack and restore
    bx      lr                  // return to caller

    //function footer
    .size encrypt, (. - encrypt) // set size for function

    //file footer - directive to turn off execution from the stack
    .section .note.GNU-stack,"",%progbits
.end
#endif
