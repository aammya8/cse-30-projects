/*
 * Programming assignment 2 sample solution
 * test frame for split_input
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "subs.h"

/* hardwired for three column records */
#define COLS 3

/*
 * main
 */ 
int 
main(int argc, char **argv)
{
    char *in_table[COLS];        /* array of pointers to column starts */
    unsigned long linecnt;  /* input line counter */
    char *buf = NULL;       /* input buffer, getline() will allocate it */
    size_t bufsz = 0;       /* size of buffer altered by getline()*/
    unsigned long dpcnt = 0;/* number of rows dropped in total */
    int retval = EXIT_SUCCESS; /* return value */
    int count = argc;       /* just keep gcc happy  not used */


    count = count + 1;
    /*
     * read the input line at a time, break into tokens and write out the
     * selected columns
     */
    linecnt = 0;
    while (getline(&buf, &bufsz, stdin) > 0) {
        linecnt++;

        /*
         * break the input into tokens point at by array in_table
         */
        if (split_input(buf, ',', COLS, in_table, linecnt, argv) != 0) {
            dpcnt++;        /* count the dropped rows */
        } else {
            for (int i = 0; i < COLS; i++)
                printf("col[%d]: %s   ",i, *(in_table + i));
            printf("\n");
       }
    }


    /*
     * if lines were dropped, write summary
     */
    if (dpcnt > 0) {
        fprintf(stderr, "%s: %lu records dropped\n", *argv, dpcnt);
        retval = EXIT_FAILURE;
    }

    /*
     * keep valgrind happy even though we are exiting and memory will be freed
     */
    free(buf);          /* was allocated by getline() */
    return retval;
}
