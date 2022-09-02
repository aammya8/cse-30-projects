#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "subs.h"
#include "misc.h"

/* field types */
#define NORMAL 1
#define QUOTED 2

/* field validity indicators */
#define VALID_FIELD 0
#define NF_QUOTE_ERR -1
#define QF_INCORRECT_TERMINATION_ERR -2
#define QF_NONTERMINATED_ERR -3


/* private (static) helper function prototype */
static int token_validator(char *start_ptr, int type);


/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to the first
 *          character of each field (column)
 * assumes: input buffer has at least one char in it (not counting the '\0')

 * args:
 *  buf     one record from read from standind input stream by getline()
 *  delim   the input field delimiter
 *  cnt     number of fields (in table)
 *  table   point to a table of pointers to the start of each field
 *  lineno  line number of record (for passing to dropmesg)
 *  argv    for drop mesg to print name of program

 * returns:  0 if all the columns were found,
 *          -1 if a record did not have all the columns
 *          -1 if a record had an bad field value
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{

    char *walk_ptr = buf; /* alias for buf */
    int col_cnt = 0;      /* current column (field) number */
    int validity;         /* return value of token_validator */
    int delim_cnt = 0;    /* counts delimiters in record */


    /* 
     * walk walk_ptr to end of record, or until all fields filled in table 
     */
    while (*walk_ptr != '\0' && col_cnt != cnt) {

        /* add field pointer to table */
        *(table + col_cnt) = walk_ptr;
        char *start_ptr = *(table + col_cnt); 


        /* 
         * Quoted field detected
         * Process quoted field
         * Validate processed field using token_validator helper function
         * */
        if (*walk_ptr == '\"') { 

            /* 
             * walk walk_ptr until closing quote is found 
             */
            while ((*walk_ptr != '\n') && (*walk_ptr != '\0')) {

                if (*(walk_ptr + 1) == '\"') {
                    /* 
                     * double quote pair found, is internal quote pair 
                     */
                    if (*(walk_ptr + 2) == '\"') {
                        walk_ptr += 2;
                        continue;
                    }


                    /* 
                     * double quote is closing quote of quoted field 
                     */

                    walk_ptr += 1;

                    /* walk to end of field (or record if last field) */
                    while ((*walk_ptr != delim) && (*walk_ptr != '\n'))
                        walk_ptr += 1;

                    /* replace end-of-field marker with '\0' */
                    if (*walk_ptr == delim) {
                        delim_cnt += 1;
                        *walk_ptr = '\0';
                    }
                    if (*walk_ptr == '\n')
                        *walk_ptr = '\0';

                    walk_ptr += 1;
                    break;
                }

                /* next char is not a double quote */
                walk_ptr += 1;
            }

            /* Validate quoted field */
            validity = token_validator(start_ptr, QUOTED);
            if (validity == QF_INCORRECT_TERMINATION_ERR) {
                dropmsg("Quoted field not terminated properly", lineno, argv);
                return -1;
            }
            if (validity == QF_NONTERMINATED_ERR) {
                dropmsg("Quoted field missing final quote", lineno, argv);
                return -1;
            }
        }


        /* 
         * Normal unquoted field detected
         * Process unquoted field
         * Validate processed field using token_validator helper function
         */
        else {

            /* walk walk_ptr to the end of field (or record if last field) */
            while ((*walk_ptr != delim) && (*walk_ptr != '\n'))
                walk_ptr += 1;
            
            /* replace end-of-field marker with '\0' */
            if (*walk_ptr == delim) {
                delim_cnt += 1;
                *walk_ptr = '\0';
            }
            if (*walk_ptr == '\n')
                *walk_ptr = '\0';

            /* move walk_ptr to start of next field (or '\0' if last field) */
            walk_ptr += 1;

            /* Validate normal unquoted field */
            validity = token_validator(start_ptr, NORMAL);
            if (validity == NF_QUOTE_ERR) {
                dropmsg("A \" is not allowed inside unquoted field", lineno, argv); 
                return -1;
            }
        }

        col_cnt++; 
    }


    /* 
     * check if there are exactly (cnt) number of fields (columns) in record 
     */
    if (delim_cnt < (cnt - 1)) {
        dropmsg("too few columns", lineno, argv);
        return -1;
    }
    if (delim_cnt > (cnt - 1)) {
        dropmsg("too many columns", lineno, argv);
        return -1;
    }
    return 0;
} 




/*
 * token_validator
 *  
 * usage:      helper function for split_input that validates a field and
 *             returns integer indicating validity or invalidity
 * assumes:    passed field terminated by '\0'
 * 
 * args:
 *  start_ptr  pointer to start of field to be validated
 *  type       type of field (NORMAL or QUOTED) 
 * 
 * returns:
 *     NF_QUOTE_ERR                  if " detected in normal field
 *     QF_INCORRECT_TERMINATION_ERR  if " not last char in quoted field
 *     QF_NONTERMINATED_ERR          if closing " in quoted field missing
 *     VALID_FIELD                   if field is valid
*/
static int
token_validator(char *start_ptr, int type)
{
    /* 
     * unquoted field validity check 
     */
    if (type == NORMAL) {
        while (*start_ptr != '\0') {
            if (*start_ptr == '\"')
                return NF_QUOTE_ERR;
            else
                start_ptr += 1;
        }
        return VALID_FIELD;
    }
    

    /* 
     * quoted field validity check 
     */
    else {
        
        /* 
         * walk to end of field and count number of quotes 
         */
        int quote_count = 0;
        while (*start_ptr != '\0') {
            if (*start_ptr == '\"')
                quote_count++;
            start_ptr += 1;
        }

        if ((quote_count % 2) != 0)
            return QF_NONTERMINATED_ERR;
        if (*(start_ptr - 1) != '\"')
            return QF_INCORRECT_TERMINATION_ERR;
        return VALID_FIELD;
    }
}




/*
 * wr_row
 *
 * usage:   given an array of pointers to columns, an array of columns to
 *          ouput, and a count of output columns
 *          outputs the columns specified in the output array (contents are
 *          index numbers of the input array) 
 *          delimiter specified by parameter char delim used as delimiter in
 *          the output records to the standard output stream
 * 
 * args:
 *  in_tab  pointer to a table of pointers to the start of each input field
 *  out_tab pointer to a table of ints containing the array index to print
 *          the order in this array is the order the cols/fields are printed
 *  out_cnt number of output record fields (number of elements in in_tab)
 *  delim   the output record delimiter to use
 *  lineno  line number of record for dropmsg
 *  argv    program name for dropmesg

 * return:  the number of records dropped during output
 *          0 if the record was printed
 *          1 if the record was dropped (special case 1 col and is empty)
 */
int
wr_row(char **in_tab, int *out_tab, int out_cnt, char delim,
        unsigned long lineno, char **argv)
{
    int idx = 0;                /* index of in_tab */
    char *st = *(in_tab + idx); /* pointer to start of field */

    /* 
     * special case: one output col, input is empty line 
     * handling: drop record (skip and do not print) 
     */
    if ((out_cnt == 1) && (*st == '\0')) {
        dropmsg("empty column", lineno, argv);
        return 1;
    }

    /* 
     * walk down output array (*out_tab) and write specified fields
     * to the standard output stream
     */
    for (int i = 0; i < (out_cnt - 1); i++) {
        idx = *(out_tab + i);
        st = *(in_tab + idx);
        printf("%s%c", st, delim);
    }
    printf("%s\n", *(in_tab + *(out_tab + (out_cnt - 1))));
    return 0;
}

