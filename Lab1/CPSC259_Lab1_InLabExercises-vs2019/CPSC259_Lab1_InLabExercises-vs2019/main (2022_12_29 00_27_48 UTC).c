/*
File:         main.c
Purpose:      Exercises for lab 1
Author:       Lachlan Reynolds
Student #s:   14511638
CS Accounts:  b2j3b
Date:         September 19th, 2022
*/

/* Preprocessor directives */
#include "lab1_inlab_exercises.h"
#include <stdlib.h> // For system command
#include <stdio.h>

/*
Try to avoid using numbers in code.  Programmers will often call
numbers in your code 'magic numbers'.  We avoid using magic numbers
because it is easier to debug code that doesn't use them.
Define and use a constant like this instead: #define CONSTANT_NAME value
*/
#define SOME_CONSTANT 10


/*
Main function drives the program.  Every C program must have one and
only one main function.  A project will not compile without one.
PRE:    NULL (no pre-conditions)
POST:	 NULL (no side-effects)
RETURN: IF the program exits correctly
THEN 0 ELSE 1
*/
int main(void)
{
    char string[] = "a nut for a jar of tuna";   

    if (is_palindrome(string) == 1) {
        printf("Palindrome!\n");
    }
    else {
        printf("Not Paliondrome :(\n");
    }
    


  return 0;
}


