//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.1 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // function import
    .extern encrypt
    .extern decrypt
    .extern setup
    .extern rdbuf
    .extern cleanup
    .extern wrbuf

    // global constants
    #include "encrypter.h"

    .text

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    //                                                  //
    // encrypter [-d | -e] -b bookfile encryption_file  //
    //////////////////////////////////////////////////////


    .global main
    .type   main, %function
    .equ    BUFSZ,      1024            // buffer size

    // set up stack frame alignments
    .equ    FP_OFF,     28              // fp offset in main stack frame
    .equ    FPBOOK,     4 + FP_OFF      // FILE * to book file
    .equ    FPIN,       4 + FPBOOK      // FILE * to input file
    .equ    FPOUT,      4 + FPIN        // FILE * to output file
    .equ    MODE,       4 + FPOUT       // decrypt or encrypt mode
    .equ    IOBUF,      BUFSZ + MODE    // char buffer for input/output file
    .equ    BOOKBUF,    BUFSZ + IOBUF   // char buffer for book file  
    .equ    PAD,        0 + BOOKBUF     // Stack frame PAD 
    .equ    OARG6,      4 + PAD         // output arg6
    .equ    OARG5,      4 + OARG6       // output arg5 (at top of stack)
    .equ    FRAMESZ,    OARG5 - FP_OFF  // total space for frame

 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to input/output buffer  //
    // r5 pointer to book buffer          //
    // r6 mode flag                       //
    // r7 input FILE * (FPIN)             //
    // r8 book FILE * (FPBOOK)            //
    // r9 output FILE * (FPOUT)           //
    ////////////////////////////////////////

main:
    // function epilogue
    push    {r4-r9, fp, lr}     // stack frame register save
    add     fp, sp, FP_OFF      // set frame pointer to frame base
    ldr     r3, =FRAMESZ        // frame size is big, use pseudo ldr
    sub     sp, sp, r3          // allocate space for locals and passed args

    // set up preserve registers for main loop
    ldr     r6, =IOBUF
    sub     r4, fp, r6          // r4 pointer to IOBUF
    ldr     r3, =BOOKBUF
    sub     r5, fp, r3          // r5 pointer to BOOKBUF
    sub     r6, fp, MODE        // r6 MODE flag
    sub     r7, fp, FPIN        // r7 FPIN
    sub     r8, fp, FPBOOK      // r8 FPBOOK
    sub     r9, fp, FPOUT       // r9 FPOUT

    // setup params and call setup function to initialize variables
    mov     r2, r6              // set arg3 to address of MODE
    mov     r3, r8              // set arg4 to address of FPBOOK
    str     r7, [fp, -OARG5]    // set arg5 to address of FPIN
    str     r9, [fp, -OARG6]    // set arg6 to address of FPOUT
                                // arg1, arg2 already argc, argv, respectively
    bl      setup               // initialize passed variables

    // exit program if variable initialization in setup failed
    cmp     r0, EXIT_FAIL       // did setup fail?
    bne     .Lloop              // if not, continue
    mov     r0, EXIT_FAILURE    // if yes, then set main return to EXIT_FAILURE
    b       .Lexit              // branch to function epilogue


.Lloop:

    //set up params for rdbuf function call
    mov     r0, BUFSZ           
    str     r0, [fp, -OARG5]    // set arg5 of rdbuf to BUFSZ
    mov     r0, r4              // set arg1 to pointer to IOBUF
    mov     r1, r5              // set arg2 to pointer to BOOKBUF
    ldr     r2, [r7]            // set arg3 to FPIN
    ldr     r3, [r8]            // set arg4 to FPOUT

    // read buffer from input and bookfile
    bl      rdbuf               

    cmp     r0, 0               // loop guard, is EOF on input file reached?
    ble     .Ldone              // if yes, branch to function prologue

    ldr     r1, [r6]            // read mode flag value into r1
    cmp     r1, ENCRYPT_MODE    // is mode encrypt?
    bne     .Ldecrypt           // if not, branch to decrypt 


.Lencrypt:

    //set up scratch registers with encrypt params
    mov     r2, r0              // set arg3 to cnt read in rdbuf
    mov     r1, r5              // set arg2 to pointer to BOOKBUF
    mov     r0, r4              // set arg1 to pointer to IOBUF

    bl      encrypt             // encrypt and update IOBUF
    b       .Lwriteout          // write out encrypted buffer


.Ldecrypt: 

    //set up scratch registers with decrypt params
    mov     r2, r0              // set arg3 to cnt read in rdbuf
    mov     r1, r5              // set arg2 to pointer to BOOKBUF
    mov     r0, r4              // set arg1 to pointer to IOBUF

    bl      decrypt             // decrypt and update IOBUF


.Lwriteout:

    cmp     r0, EXIT_FAIL       // did XXcrypt return EXIT_FAIL?
    beq     .Ldone              // if so, return EXIT_FAILURE from main

    //set up registers and call wrbuf function 
    ldr     r2, [r9]            // set arg3 to FPOUT
    mov     r1, r0              // set arg2 to cnt
    mov     r0, r4              // set arg1 to pointer to IOBUF
    bl      wrbuf               // write out XXecrypted buffer

    cmp     r0, EXIT_FAIL       // did wrbuf return EXIT_FAIL?  
    bne     .Lloop              // if not, read next buffer

.Ldone:
    // arg1 of cleanup, r0 has exit status (cnt or EXIT_FAIL)
    bl      cleanup             // close files

.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r9, fp, lr}     // remove frame from stack and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function

    // file footer - directive to turn off execution from the stack
    .section .note.GNU-stack,"",%progbits
.end
#endif
