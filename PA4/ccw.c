#include <stdio.h>
#include <stdlib.h>
#include "cw.h"

/* state of buffer ptr (delimiter or countable character) */
#define DELIM_ST 0
#define CHAR_ST 1

char buffer[BUFSZ];  /* buffer that rd() fills */
int  cntchar;        /* count of chars IN words,not \n, \t or ' ' */
int  cntword;        /* number of words */
int  cntline;        /* number of \n characters */

int setup(int, char **);
int rd(void);
void result(void); 


/*
 * main
 * 
 * usage:      ccw [file]
 * operation:  Reads a buffer at a time from [file] specified on command line
 *             or from stdin if ccw run with no arguments. Counts and prints
 *             a summary of the number of newline characters, number of words,
 *             and number of non-delimiter ('\n', ' ', '\t') chars in input.
 * 
 * args:       argc, argv
 * 
 */
int
main(int argc, char **argv)
{
    if (setup(argc, argv) != 0)
        return EXIT_FAILURE;

    int byte_cnt;          /* stores number of bytes stored in buffer   */
    int state = DELIM_ST;  /* state of buffer ptr (delimiter/character) */


    /*
     * Outer loop. Calls rd() to get a buffer of chars (and number count of 
     * valid chars in buffer) in loop until EOF.
     */
    while((byte_cnt = rd()) > 0) {
        char *buf_ptr = buffer;              /* ptr to walk through buffer */
        char *end_ptr = buf_ptr + byte_cnt;  /* ptr to end of buffer + 1 */

        /* 
         * Inner loop. Walks buf_ptr to end of buffer, updating cntchar, 
         * cntword, cntline. state var used to determine start of word
         */
        while (buf_ptr < end_ptr) {
            if (*buf_ptr == '\n') {
                cntline += 1;
                state = DELIM_ST;
            }
            else if ((*buf_ptr == ' ') || (*buf_ptr == '\t')) {
                state = DELIM_ST;
            }
            else {

                /* 
                 * prev char was delimiter
                 * new word has been found
                 */
                if (state == DELIM_ST) {
                    state = CHAR_ST;
                    cntword += 1;
                }

                cntchar += 1;
            }
            buf_ptr = buf_ptr + 1;
        }
    }




//#define _PA4DBG_
#ifdef _PA4DBG_
     printf("lines:%8.8d words:%8.8d Chars:%8.8d\n", cntline,cntword,cntchar);
#else
    result();
#endif
    return EXIT_SUCCESS;
}
