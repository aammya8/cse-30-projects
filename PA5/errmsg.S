//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.01 05/27/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    // imports
    .extern stderr
    .extern fprintf

    .text

    ////////////////////////////////////////////
    // void errmsg(char *errormsg)            //
    //                                        //
    // writes error messages to stderr        //
    ////////////////////////////////////////////

    .type   errmsg, %function
    .global errmsg
    .equ    FP_OFF,  4

errmsg:
    // function prologue
    push    {fp, lr}            // stack frame register save
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    // set up params and call fprintf function
    mov     r1, r0              // set arg1 to pointer to error message
    ldr     r0, =stderr         // pseudo ldr
    ldr     r0, [r0]            // set arg1 to stderr
    bl      fprintf             // write error message to stderr

    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {fp, lr}            // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size errmsg, (. - errmsg)  // set size for function

    // file footer - directive to turn off execution from the stack
    .section .note.GNU-stack,"",%progbits
.end
#endif
