//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.1 05/22/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "encrypter.h"

    .section .rodata
.Lmsg:  .string "rdbuf: Bookfile is too short for message\n"
.Lmsg2: .string "rdbuf: Buffer size error\n"

    .text

    //////////////////////////////////////////////////////
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN //
    //              FILE *FPBOOK, int BUFSZ);           //
    //                                                  //
    // returns number of bytes read EXIT_FAIL otherwise //
    //////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    .equ    IARG5,  4 
    .equ    FP_OFF, 12
 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to book buffer          //
    // r5 FILE *FPBOOK  & cnt read saved  //
    ////////////////////////////////////////

rdbuf:
    // function prologue
    push    {r4, r5, fp, lr}    // stack frame register save
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    mov     r4, r1              // move pointer to BOOKBUF to r4
    mov     r5, r3              // move FPBOOK to r5

    // check if BUFSZ is invalid 
    ldr     r1, [fp, IARG5]     // load BUFSZ into r1
    cmp     r1, 0               // is BUFSZ > 0?
    ble     .Lerr1              // if not, process error

    // set up params for fread function call on input file
    mov     r3, r2              // set arg4 to FPIN
    mov     r2, r1              // set arg3 to BUFSZ
    mov     r1, 1               // set arg2 to 1
                                // arg1 already set to ptr to IOBUF

    // read input file buffer, returns # chars read
    bl      fread               

    cmp     r0, 0               // has the EOF been reached?
    beq     .Ldone              // if so, return cnt of 0 chars read


    // set up params for fread call on bookfile
    mov     r3, r5              // set arg4 to FPBOOK
    mov     r2, r0              // set arg3 to cnt chars read from input file
    mov     r1, 1               // set arg2 to 1
    mov     r0, r4              // set arg1 to ptr to BOOKBUF

    // save cnt of chars read in input file in r5
    mov     r5, r2              

    // read same number of chars of bookfile as from input file
    bl      fread               

    // check if bookfile is long enough for message
    cmp     r0, r5              // was same cnt of chars read in bookfile?
    bne     .Lerr2              // if not, process error
    mov     r0, r5              // if no error, set rdbuf return value to cnt
    b       .Ldone              // branch to function epilogue


.Lerr1:
    ldr     r0, =.Lmsg2         // set up param for errmsg call
    bl      errmsg              // print invalid BUFSZ error message
    mov     r0, EXIT_FAIL       // set rdbuf return value to EXIT_FAIL
    b       .Ldone              // branch to function epilogue

.Lerr2:
    ldr     r0, =.Lmsg          // set up param for errmsg call
    bl      errmsg              // print invalid bookfile error message 
    mov     r0, EXIT_FAIL       // set rdbuf return value to EXIT_FAIL
         
.Ldone:
    //function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4, r5, fp, lr}    // remove frame from stack and restore
    bx      lr                  // return to caller

    // function footer
    .size   rdbuf, (. - rdbuf)  // set size for function

    // file footer - directive to turn off execution from the stack
    .section .note.GNU-stack,"",%progbits
.end
#endif
