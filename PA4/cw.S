    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    //  include files
    #include    "cw.h"

    // name imports
    .extern     setup
    .extern     rd
    .extern     result
    
    // name exports
    .global     buffer
    .global     cntchar
    .global     cntword
    .global     cntline


    .data       // segment
cntchar:    
    .word  0                // count of non-delimiter chars
    .align 2
cntword:    
    .word  0                // count of only words
cntline:    
    .word  0                // count of newline \n
buffer:     
    .space BUFSZ            // buffer of chars
    .align 2                // in case BUFSZ not a multiple of 4



    .text       // segment

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // program usage: cw [file]                         //
    //                                                  //
    // scratch registers use in main:                   //
    //   r0  will hold return value of rd               //
    //   r1  will point to and walk through buffer      //
    //   r2  will point after the end of the buffer     //
    //   r3  will hold int denoting state of current    //
    //       buffer char. 0 represents delimiter, 1     //
    //       represents valid countable character       //
    //                                                  //
    // preserved registers use in main:                 //
    //   r4  will hold non-delim char count             //
    //   r5  will hold word count                       //
    //   r6  will hold newline count                    //
    //   r7  will hold contents of buffer[0]            //
    //                                                  //
    //                                                  //
    //////////////////////////////////////////////////////

    .global main                // global for linking to
    .type   main, %function     // define as a function
    .equ    EXIT_SUCCESS, 0
    .equ    EXIT_FAILURE, 1
    .equ    FP_OFF, 20          // fp offset in stack frame

main:
    // function prologue
    push    {r4-r7,fp, lr}      // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    bl      setup               // call setup(argc, argv)
    cmp     r0, 0               // returns 0 if ok, -1 otherwise
    beq     .Lok
    mov     r0, EXIT_FAILURE
    b       .Lexit

.Lok:   

    // initialize r3-r6 to hold values of counter and state variables 
    ldr     r4, =cntchar
    ldr     r4, [r4]
    ldr     r5, =cntword
    ldr     r5, [r5]
    ldr     r6, =cntline
    ldr     r6, [r6]
    mov     r3, 0               


.Louterloop:

    bl      rd                  // read another buffer
    cmp     r0, 0               // if r0 <= 0 then EOF
    ble     .Ldone  

    ldr     r1, =buffer         // r1 points to buffer
    add     r2, r0, r1          // r2 points after end of buffer


.Linnerloop:
    cmp     r1, r2              // check if buffer ptr at end of buffer
    bge     .Louterloop         // if so, read another buffer

    ldrb r7, [r1]               // load contents of buffer ptr into r7


    // process buffer ptr pointing at delimiter
    cmp     r7, '\n'            // is buffer ptr pointing at \n char?
    beq     .Lcntline           // if so, \n delim found. update variables
    cmp     r7, ' '             // is buffer ptr pointing at ' ' char?
    beq     .Ldelimstate        // if so, ' ' delim found. update variables 
    cmp     r7, '\t'            // is buffer ptr pointing at '\t' char?
    beq     .Ldelimstate        // if so, '\t' delim found. update variables


    // process buffer ptr pointing at countable character
    add     r4, r4, 1           // increment character count
    cmp     r3, 0               // is previous char a delimiter?
    beq     .Lwordfound         // if so, new word found. update word count
    mov     r3, 1               // change state to 1 to indicate non-delim char
    add     r1, r1, 1           // walk buffer ptr to next char in buffer
    b       .Linnerloop         


.Lcntline:
    add     r6, r6, 1           // increment new line count
    mov     r3, 0               // change state to 0 to indicate delim char
    add     r1, r1, 1           // walk buffer ptr to next char in buffer
    b       .Linnerloop         


.Ldelimstate:
    mov     r3, 0               // change state to 0 to indicate delim char
    add     r1, r1, 1           // walk buffer ptr to next char in buffer
    b       .Linnerloop         


.Lwordfound:
    add     r5, r5, 1           // increment word count
    mov     r3, 1               // change state to 1 to indicate non-delim char
    add     r1, r1, 1           // walk buffer ptr to next char in buffer
    b       .Linnerloop         


.Ldone:

    //store count values into  globals for cntchar, cntword, and cntline
    ldr     r1, =cntchar
    str     r4, [r1]
    ldr     r2, =cntword
    str     r5, [r2]
    ldr     r3, =cntline
    str     r6, [r3]

    bl      result              // print the results
    mov     r0, EXIT_SUCCESS    // fall through to .Lexit

.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r7,fp,lr}       // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function
.end
