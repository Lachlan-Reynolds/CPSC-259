
/*HEADER FILES*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

#define  BUFSIZE 256
#define FILE_NAME "web.txt"

/*Function Definitions from Lab 4*/
int get_maze_dimension(FILE*);
int** parse_maze(FILE*, int);

int main(void) {

    /*MATLAB Variables*/
    Engine* ep = NULL;                                                      // A pointer to a MATLAB engine object
    mxArray* connectivity = NULL;
    mxArray* dimensions = NULL;      // mxArray is the fundamental type underlying MATLAB data
    mxArray* result = NULL;

    /*Matrix Variables*/
    double tempMatrix[BUFSIZE][BUFSIZE];
    double matrixOne[BUFSIZE * 235];
    char buffer[BUFSIZE + 1];
    FILE* file_ptr = NULL;
    int** connectivity_matrix = NULL;

    /*Other Variables*/
    int i=0;
    int j=0;
    int k = 0;
    int dimension = 0;
    int row = 0;
    int column = 0;

    /*Step1: Check if text file is valid and copy into 2D dynamic Memory like in Lab 4*/
    file_ptr = fopen(FILE_NAME, "r");
        
    /*File is Valid*/
    if (file_ptr) {
        dimension = get_maze_dimension(file_ptr);

        /*Dimension won't work if less than 2*/
        if (dimension < 2) {
            printf("Dimension is too small, must be at least 2\n");
            system("pause");
            return 1;           
        }

        /*Get connectivity matrix like in Lab 4 and store in tempMatrix*/
        connectivity_matrix = parse_maze(file_ptr, dimension);

        for (row = 0; row < dimension; row++) {

            for (column = 0; column < dimension; column++) {

                tempMatrix[row][column] = (double)connectivity_matrix[row][column] - 48;
            }
        }

    }

    /*File is invalid*/
    else {
        fprintf(stderr, "Unable to parse web: %s\n", FILE_NAME);
        system("pause");
        return 1;
    }

    /*Step 2: Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }


    /*Send connectivity matrix to matlab*/
    connectivity = mxCreateDoubleMatrix(dimension, dimension, mxREAL);

    /*One dimensionalize 2D array contained connectivity matrix*/
    for (i = 0; i < dimension; i++) {

        for (j = 0; j < dimension; j++) {

            matrixOne[k] = tempMatrix[i][j];
            k++;
        }
        j = 0;
    }

    /*Copy 1D arrary into matlab matrix*/
    memcpy((void*)mxGetPr(connectivity),(void*)matrixOne,dimension*dimension*sizeof(double));

    if (engPutVariable(ep, "connectivity", connectivity)) {
        fprintf(stderr, "\nCannot write connectivity matrix into MATLAB\n");
        system("pause");
        exit(1);
    }

    /*Send dimension to MATLAB*/
    dimensions = mxCreateDoubleScalar(dimension);

 
    
    /*Dimension Initialize
    if (engPutVariable(ep, "dimensions", dimensions)) {
        fprintf(stderr,"\nCannot write dimensions to MATLAB\n");
        system("pause");
        exit(1);
    }*/

    /*Probability factor Initialize*/
    if (engEvalString(ep, "p = 0.85")) {
        fprintf(stderr,"\nError entering probability factor\n");
        system("pause");
        exit(1);

    }

    /*Checking row and column of connectivity matrix in matlab*/
    if (engEvalString(ep, "[rows, columns] = size(connectivity)")) {
        fprintf(stderr, "\nError calculating rows and columns");
        system("pause");
        exit(1);
    }

    /*Dimension Initialize*/
    if (engEvalString(ep, "dimension = size(connectivity , 1)")) {
        fprintf(stderr, "\nError calculating dimension  \n");
        system("pause");
        exit(1);
    }

    /*calculate column sum and stores it in an array*/
    if (engEvalString(ep, "columnsums = sum(connectivity,1)")) {
        fprintf(stderr, "\nError calculating columnsums  \n");
        system("pause");
        exit(1);
    }

    /*Find indices of columns that sum don't sum to 0*/
    if (engEvalString(ep, "zerocolumns = find(columnsums~=0)")) {
        fprintf(stderr, "\nError finding columnsums  zero sum indices  \n");
        system("pause");
        exit(1);
    }

    /*Gets inverse matrix D (inverse vector of sums), element wise operation. */
    if (engEvalString(ep, "D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension )")) {
        fprintf(stderr, "\nError calculating sparse (inverse) matrix  \n");
        system("pause");
        exit(1);
    }

    /*Gets Stochastic (transition) matrix*/
    if (engEvalString(ep, "StochasticMatrix = connectivity * D")) {
        fprintf(stderr, "\nError calculating Stochastic matrix  \n");
        system("pause");
        exit(1);
    }

    /*Finds the zero column in original connectivity */
    if (engEvalString(ep, "[row, column] = find(columnsums==0)")) {
        fprintf(stderr, "\nError calculating zero columns \n");
        system("pause");
        exit(1);
    }

    /*Generates stochastic matrix, elementwise operation*/
    if (engEvalString(ep, "StochasticMatrix(:, column) = 1./dimension")) {
        fprintf(stderr, "\nError generating Stochastic matrix \n");
        system("pause");
        exit(1);
    }
    
    /*Generates matrix with equal dimension and entries of D inverse*/
    if (engEvalString(ep, "Q = ones(dimension, dimension)")) {
        fprintf(stderr, "\nError calculating  Q \n");
        system("pause");
        exit(1);
    }

    /*Generates Transition matrix*/
    if(engEvalString(ep, "TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension)")) {
        fprintf(stderr, "\nError calculating Transition matrix \n");
        system("pause");
        exit(1);
    }
    
    /*Generates Raw Pagerank*/
    if (engEvalString(ep, "PageRank = ones(dimension, 1)")) {
        fprintf(stderr, "\nError calculating Page rank \n");
        system("pause");
        exit(1);
    }

    /*Continuously multiplies column vector by transition matrix*/
    if (engEvalString(ep, "for i = 1:100 PageRank = TransitionMatrix * PageRank; end")) {
        fprintf(stderr, "\nError calculating for Page rank until column vectors stopped changing \n");
        system("pause");
        exit(1);
    }

    /*Normalizes pagerank*/
    if (engEvalString(ep, "PageRank = PageRank / sum(PageRank)")) {
        fprintf(stderr, "\nError calculating normalized Page rank vector \n");
        system("pause");
        exit(1);

    }
    printf("\nRetrieving Page Ranks...\nPlease wait...\n\n");

    /*Get eigenvalues*/
    if ((result = engGetVariable(ep, "PageRank")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve eigenvalue vector\n");
        system("pause");
        exit(1);
    }

    /*Print PageRanks*/
    else {
        size_t sizeOfResult = mxGetNumberOfElements(result);
        size_t i = 0;

        printf("The PageRanks are:\n");
        for (i = 0; i < sizeOfResult; ++i) {
            printf("Page %d: %f\n", i+1,*(mxGetPr(result) + i));
        }
    }

    printf("\n");

    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
        system("pause");
        return 1;
    }

    buffer[BUFSIZE] = '\0';
    engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
    printf("%s\n", buffer);

    mxDestroyArray(connectivity);
    mxDestroyArray(result);
    mxDestroyArray(dimensions);

    connectivity = NULL;
    dimensions = NULL;
    result = NULL;

    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }
    printf("Thank you! :D\n");

    system("pause"); // So the terminal window remains open long enough for you to read it
	return 0;
}


/*Function Implementations*/

int get_maze_dimension(FILE* maze_file) {

    int  dimension = 0;
    char line_buffer[BUFSIZE];

    dimension = strlen(fgets(line_buffer, BUFFER, maze_file));

    /* You don't need to know this.  It 'resets' the file's internal pointer to the
       beginning of the file. */
    fseek(maze_file, 0, SEEK_SET);

    /* Checks if text file was created in Windows and contains '\r'
       IF TRUE reduce strlen by 2 in order to omit '\r' and '\n' from each line
       ELSE    reduce strlen by 1 in order to omit '\n' from each line */
    if (strchr(line_buffer, '\r') != NULL) {
        return dimension - 2;
    }

    else {
        return dimension - 1;
    }
}

int** parse_maze(FILE* file_pointer, int dimension)
{
    /* Variables */
    char         line_buffer[BUFSIZE];
    int          row = 0;
    int 	   column = 0;
    int** connectivity_matrix = NULL;

    /* Allocates memory for correctly-sized maze */
    connectivity_matrix = (int**)calloc(dimension, sizeof(int*));


    for (row = 0; row < dimension; ++row) {
        connectivity_matrix[row] = (int*)calloc(dimension, sizeof(int));
    }

    row = 0;
    while (fgets(line_buffer, BUFSIZE, file_pointer)) {
        for (column = 0; column < dimension; ++column) {
            //allows for spaces in txt file
            connectivity_matrix[row][column] = line_buffer[column * 2] - '0';
        }
        row++;
    }
    return connectivity_matrix;
}