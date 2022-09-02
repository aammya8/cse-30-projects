//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.0 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified
    
    // import constants
    #include "encrypter.h"

    .section .rodata
.Lmsg: .string "wrbuf: Write failed on output\n"
.Lmsg2: .string "wrbuf: Buffer size error\n"

    .text

    ////////////////////////////////////////////////////
    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT);  //
    //                                                //
    // return cnt on success EXIT_FAIL otherwise      //
    ////////////////////////////////////////////////////

    .type   wrbuf, %function
    .global wrbuf
    .equ    FP_OFF, 12

    //////////////////////////////////////////
    // preserved register use:              //
    // r4 holds cnt of chars to be written  //
    //////////////////////////////////////////
 
wrbuf:
    // function prologue
    push    {r4, r5, fp, lr}    // stack frame register save
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    // check if cnt of chars to write is valid
    cmp     r1, 0               // is cnt > 0?
    ble     .Lerr1              // if not, process error

    mov     r4, r1              // save cnt of chars to be written in r4 

    // set up params for fwrite function call
    mov     r3, r2              // set arg4 to FPOUT
    mov     r2, r1              // set arg3 to cnt
    mov     r1, 1               // set arg2 to 1
                                // arg1 already set to ptr to IOBUF

    // write chars to output, return number of chars written
    bl      fwrite

    // check if write was successful
    cmp     r0, r4              // were all cnt chars written to output?
    bne     .Lerr2              // if not, process error
    mov     r0, r4              // if no error, set wrbuf return value to cnt
    b       .Ldone              // branch to function epilogue

.Lerr1:
    ldr     r0, =.Lmsg2         // set up param for errmsg call
    bl      errmsg              // print buffer size error message
    mov     r0, EXIT_FAIL       // set wrbuf return value to EXIT_FAIL
    b       .Ldone              // branch to function epilogue

.Lerr2:
    ldr     r0, =.Lmsg          // set up param for errmsg call
    bl      errmsg              // print write fail error message
    mov     r0, EXIT_FAIL       // set wrbuf return value to EXIT_FAIL

.Ldone:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4, r5, fp, lr}    // remove frame from stack and restore
    bx      lr                  // return to caller

    // function footer
    .size   wrbuf, (. - wrbuf)  // set size for function

    // file footer - directive to turn off execution from the stack
    .section .note.GNU-stack,"",%progbits
.end
#endif
