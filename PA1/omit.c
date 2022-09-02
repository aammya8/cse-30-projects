/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"


/*
 * function main()
 *
 * Operation: iterates through characters in source file and calls
 * appropriate subroutine function based on current state of DFA.
 * Keeps track of line number and prints error message for specified
 * syntax errors.
 * param: void
 * returns: EXIT_FAILURE if at illegal state when EOF reached, EXIT_SUCCESS
 * otherwise
 */
int
main(void)
{
    int c;  /* input char */
    int linecnt = 1;  /* counts line in input */
    int startline = 1;  /* starting line number for comment/quote */

    enum typestate state = START;  /* initial state of DFA */

    /*
     * process char at a time until EOF
     */
    while ((c = getchar()) != EOF) {
        switch (state) {
	case START:
	    /*
	     * start state
	     */
	    state = STARTst(c);

	    /* if character or string literal found, update startline */
	    if (state == SQUOTE || state == DQUOTE) 
		startline = linecnt;

	    break;
	case SQUOTE:
	    /*
	     * opening single quote has been seen
	     */
	    state = SQUOTEst(c);
	    break;
	case DQUOTE:
	    /*
	     * opening double quote has been seen
	     */
	    state = DQUOTEst(c);
	    break;
	case SQIGN:
	    /*
	     * backslash has been seen inside single quote, ignore next char
	     */
	    state = SQIGNst(c);
	    break;
	case DQIGN:
	    /*
	     * backslash has been seen inside double quote, ignore next char
	     */
	    state = DQIGNst(c);
	    break;
	case SLASH1:
	    /*
	     * single forward slash has been seen, not comfirmed to be comment
	     */
	    state = SLASH1st(c);

	    /* if C++ style of C style comment found, update startline */
	    if (state == SLASH2 || state == SLASHSTAR) 
		startline = linecnt;

	    break;
	case SLASH2:
	    /*
	     * two successive forward slashes seen, C++ style comment
	     */
	    state = SLASH2st(c);
	    break;
	case SLASHSTAR:
	    /*
	     * successive forward slash and * seen, looking for closing *
	     */
	    state = SLASHSTARst(c);
	    break;
	case STARSLASH:
	    /*
	     * opening forward slash and * plus closing * seen
	     * looking for closing forward slash
	     */
	    state = STARSLASHst(c);
	    break;
	default:
	    /*
	     * never reached unless a state has been added and not handled
	     */
	    fprintf(stderr, "Program error, DFA state not handled\n");
	    return EXIT_FAILURE;

	    /* NOT REACHED */
	    break;
	}

	/* update linecnt */
	if (c == '\n') 
	    linecnt++;

    } /* end of file reached */


    /* handle file ending on a forward slash */
    if (state == SLASH1)
	putchar('/');
    


    /*
     * check ending state to detect syntax error situation
     */
    if (state == SLASHSTAR || state == STARSLASH) {
	fprintf(stderr, "Error: line %d: unterminated comment\n", startline);
	return EXIT_FAILURE;
    }
    if (state == SQUOTE) {
	fprintf(stderr, "Error: line %d: unterminated quote(\')\n", startline);
	return EXIT_FAILURE;
    }
    if (state == DQUOTE) {
	fprintf(stderr, "Error: line %d: unterminated quote(\")\n", startline);
	return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;

}
