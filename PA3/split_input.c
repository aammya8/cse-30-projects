#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "misc.h"
#include "hashdb.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * split_input
 * 
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * returns:  0 if all the columns were found,  -1 otherwise
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
/* insert your code here */
}
#endif
