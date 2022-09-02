/* 
 * the next two lines are the header guards
 */
#ifndef _OMIT_H
#define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 */
enum typestate {START, SQUOTE, SQIGN, DQUOTE, DQIGN, SLASH1, SLASH2,
	        SLASHSTAR, STARSLASH};

/*
 * function prototypes for each state of the DFA
 */
enum typestate STARTst(int);
enum typestate SQUOTEst(int);
enum typestate SQIGNst(int);
enum typestate DQUOTEst(int);
enum typestate DQIGNst(int);
enum typestate SLASH1st(int);
enum typestate SLASH2st(int);
enum typestate SLASHSTARst(int);
enum typestate STARSLASHst(int);

#endif
