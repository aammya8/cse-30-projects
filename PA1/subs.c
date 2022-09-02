/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"

/*
 * function STARTst()
 *
 * Operation: handles start state
 * param: current char in input
 * returns: resulting state
 */
enum typestate
STARTst(int c)
{
    if (c == '\'') {
	putchar('\'');
        return SQUOTE;
    }
    else if (c == '\"') {
	putchar('\"');
	return DQUOTE;
    }
    else if (c == '/') {
	return SLASH1;
    }
    else {
	putchar(c);
	return START;
    }
}

/*
 * function SQUOTEst()
 *
 * Operation: handle character literals
 * param: current char in input
 * returns: resulting state
 */
enum typestate
SQUOTEst(int c)
{
    if (c == '\\') {
	putchar('\\');
	return SQIGN;
    }
    else if (c == '\'') {
	putchar('\'');
	return START;
    }
    else {
	putchar(c);
	return SQUOTE;
    }
}

/*
 * function SQIGNst()
 *
 * Operation: handles backslash in a character literal,
 * e.g. ignores special meaning to the following char
 * param: current char in input
 * returns: SQUOTE state
 */
enum typestate
SQIGNst(int c)
{
    putchar(c);
    return SQUOTE;
}

/*
 * function DQUOTEst()
 *
 * Operation: handle string literals
 * param: currrent char in input
 * returns: resulting state
 */
enum typestate
DQUOTEst(int c)
{
    if (c == '\\') {
	putchar('\\');
	return DQIGN;
    }
    else if (c == '\"') {
	putchar('\"');
	return START;
    }
    else {
	putchar(c);
	return DQUOTE;
    }
}

/*
 * function DQIGNst()
 *
 * Operation: handles backslash in a string literal,
 * e.g. ignores special meaning to the following char
 * param: current char in input
 * returns: DQUOTE state
 */
enum typestate
DQIGNst(int c)
{
    putchar(c);
    return DQUOTE;
}

/*
 * function SLASH1st()
 *
 * Operation: handle state where single slash has been seen
 * param: current char in input
 * returns: resulting state
 */
enum typestate
SLASH1st(int c)
{
    if (c == '/') {
	putchar(' ');
	return SLASH2;
    }
    else if (c == '*') {
	putchar(' ');
	return SLASHSTAR;
    }
    else if (c == '\'') {
	putchar('/');
	putchar('\'');
	return SQUOTE;
    }
    else if (c == '\"') {
	putchar('/');
	putchar('\"');
	return DQUOTE;
    }
    else {
	putchar('/');
	putchar(c);
	return START;
    }
}

/*
 * function SLASH2st()
 *
 * Operation: handles case where two successive slashes (at least)
 * have been seen, so in a C++ style comment
 * param: current char in input
 * returns: resulting state
 */
enum typestate
SLASH2st(int c)
{
    if (c == '\n') {
	putchar('\n');
	return START;
    }
    else {
	return SLASH2;
    }
}

/*
 * function SLASHSTARst()
 *
 * Operation: handles case where an opening forward slash and *
 * have been seen (so in C-style comment) looking for closing *
 * param: current char in input
 * returns: resulting state
 */
enum typestate
SLASHSTARst(int c)
{
    if (c == '*') {
	return STARSLASH;
    }
    else if (c == '\n') {
	putchar('\n');
	return SLASHSTAR;
    }
    else {
	return SLASHSTAR;
    }
}

/*
 * function STARSLASHst()
 *
 * Operation: handles case where an opening forward slash and *,
 * plus closing * of C-style comment have been seen (looking for
 * closing forward slash)
 * param: current char in input
 * returns: resulting state
 */
enum typestate
STARSLASHst(int c)
{
    if (c == '/') {
	return START;
    }
    else if (c == '\n') {
	putchar('\n');
	return SLASHSTAR;
    }
    else {
	return SLASHSTAR;
    }
}


