/**
 * Michael Anderson
 * CS433
 * Homework 1 (Linux Assembly Inlining)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRORMSG "Please include exactly THREE (3) command line arguments: \n"\
				 "1. A string of text \n"\
				 "2. The beginning index of the substring to be lifted from the string of text \n"\
				 "3. The ending index of the subtraing to be lifted from the string of text \n"\
				 "Exiting... \n"
#define MAX_STRING_LENGTH 99

static inline char * sub_str(char *dest, char *src, int s_idx, int e_idx){
	int charsToLift = e_idx - s_idx;
	int i;

	for (i = 0; i < charsToLift; i++) {
		dest[i] = src[s_idx + i];
	}
	dest[i] = 00; // Add null terminator (octal 0) to end of substring
	return dest;
}

static inline char * asm_sub_str(char *dest, const char *src, int s_idx, int e_idx){
	int d0, d1, d2;
	__asm__ __volatile__(  "addw\t%5,%%si\n\t" 			/* Increment memory address by value of s_idx */
				   		   "movw\t%6,%%cx\n\t"			/* Move value of e_idx to cx */
						   "subw\t%5,%%cx\n\t"			/* Subtract value of s_idx from cx for counting loop */

						   "1: testw %%cx,%%cx\n\t"		/* See if the value of cx is 0; if so, zero flag gets set */
						   "je 3f\n\t"					/* If zero flag is set, jump forward to 3 (break out of loop)*/
						   "dec %%cx\n\t"				/* Decrement cx */
 
						   "lodsb\n\t" 					/* Load string byte at ESI and put in EAX */
						   "stosb\n\t"					/* Store string byte in EAX at EDI */
						   
                           "2:\ttestb %%al,%%al\n\t"	/* See if the value of al is 0; if so, set zero flag */
						   "jne 1b\n\t"					/* Jump back to 1 if zflag not set */
															/* Null-terminate the substring: */
						   "3:\tmovl\t$0x0000,%%eax\n\t"	/* Move 0x0 into ax */	
						   "stosb\n\t"						/* Store null byte in EAX at EDI */

						   "4:\tnop"
						 : "=&S" (d0), "=&D" (d1), "=&a" (d2)
						 : "0" (src), "1" (dest), "g" (s_idx), "g" (e_idx) /* NB: Had to set constraint for s_idx to g to get it to work */ 
						 : "memory");
	return dest;
}

/* String copy funtion direct from ibiblio.org, for reference: */
/* URL: http://ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html */
static inline char * mystringcopy(char *dest, const char *src) {
	int d0, d1, d2;
	__asm__ __volatile__(  "1:\tlodsb\n\t"
						   "stosb\n\t"
                           "testb %%al,%%al\n\t"
						   "jne 1b"																                      		 
						 : "=&S" (d0), "=&D" (d1), "=&a" (d2)
						 : "0" (src),"1" (dest) 
					     : "memory");
		return dest;
}

int main(int argc,char **argv) {
	char destBuf1[100];
	char destBuf2[100];
	int startIndex, endIndex;
	
	printf("Michael Anderson | CS433 | Homework 1 (Linux Assembly Inlining)\n");	
	/* get a string and starting and ending indices from the command line */
	if (argc != 4) {
		printf(ERRORMSG);
		return 0;
	} else {
		if (strlen(argv[1]) > MAX_STRING_LENGTH) {
			printf("Length of argv[1] too long.  Length is %ld.  Max length is 99 characters.\n", strlen(argv[1]));
			return 0;
		} else if (atoi(argv[2]) > atoi(argv[3])) {
			printf("Starting index cannot be greater than ending index.");
			return 0;
		} else if (atoi(argv[2]) < 0 ||  
				   atoi(argv[3]) < 0 ) {
			printf("Indicies cannot be negative");
			return 0;
		} else if (atoi(argv[2]) > strlen(argv[1]) ||  
				   atoi(argv[3]) > strlen(argv[1]) ) {
			printf("Indicies cannot be greater than the length of the length of the string.");
			return 0;
		} 
	}
	startIndex = atoi(argv[2]);
	endIndex = atoi(argv[3]);
	// printf("Length of argv[1]: %ld\n", strlen(argv[1]));
	printf("Value of argv[1]: %s\n", argv[1]);
	printf("Value of startIndex: %d\n", startIndex);
	printf("Value of endIndex: %d\n", endIndex);
	// printf("Value of destBuf1: %s\n", destBuf1);
	// printf("Value of destBuf2: %s\n", destBuf2);
	/* call an inline C function */
	printf("Calling sub_str(destBuf1, argv[1], startIndex, endIndex)...\n");
	sub_str(destBuf1, argv[1], startIndex, endIndex);
	printf("Value of destBuf1 (after inline C function): %s\n", destBuf1);	
	/* call a C function, d2=asm_sub_str(d,s,s_idx,e_idx) using inlining (assembly) */
	printf("Calling asm_sub_str(destBuf2, argv[1], startIndex, endIndex) using inline assembly...\n");
	asm_sub_str(destBuf2, argv[1], startIndex, endIndex);
	printf("Value of destBuf2 (after inline assembly function): %s\n", destBuf2);
	/* compare the two strings d1 and d2 and return the result */
	printf("Comparing the strings at destBuf1 and destBuf2 using strcmp()...\n");	
	if (!strcmp(destBuf1, destBuf2)) {
		printf("strcmp(destBuf1, destBuf2) returned 0.  The strings are equal.\n");
	} else {
		printf("I hope this code never runs.");
	}
	return 0;
}
