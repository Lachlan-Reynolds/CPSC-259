
/*HEADER FILES*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

#define  BUFSIZE 256


int main(void) {

    /* Variables */
    Engine* ep = NULL;                          // A pointer to a MATLAB engine object
    mxArray* testArray = NULL, * result = NULL; // mxArray is the fundamental type underlying MATLAB data
    double time[3][3] = { { 1.0, 2.0, 3.0 }, {4.0, 5.0, 6.0 }, {7.0, 8.0, 9.0 } }; // Our test 'matrix', a 2-D array
    char buffer[BUFSIZE + 1];


    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }

    testArray = mxCreateDoubleMatrix(3, 3, mxREAL);

    memcpy((void*)mxGetPr(testArray), (void*)time, 9 * sizeof(double));

    if (engPutVariable(ep, "testArray", testArray)) {
        fprintf(stderr, "\nCannot write test array to MATLAB \n");
        system("pause");
        exit(1); // Same as return 1;
    }

    if (engEvalString(ep, "testArrayEigen = eig(testArray)")) {
        fprintf(stderr, "\nError calculating eigenvalues  \n");
        system("pause");
        exit(1);
    }

    printf("\nRetrieving eigenvector\n");
    if ((result = engGetVariable(ep, "testArrayEigen")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve eigenvalue vector\n");
        system("pause");
        exit(1);
    }
    else {
        size_t sizeOfResult = mxGetNumberOfElements(result);
        size_t i = 0;
        printf("The eigenvalues are:\n");
        for (i = 0; i < sizeOfResult; ++i) {
            printf("%f\n", *(mxGetPr(result) + i));
        }
    }

    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
        system("pause");
        return 1;
    }
    buffer[BUFSIZE] = '\0';
    engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
    printf("%s\n", buffer);

    mxDestroyArray(testArray);
    mxDestroyArray(result);
    testArray = NULL;
    result = NULL;
    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0;
}

