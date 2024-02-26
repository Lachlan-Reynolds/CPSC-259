/*
File:         tide_analyzer.c
Purpose:      Consumes a semi-formatted tide measurement file and
determines if the corresponding tide is once- or
twice-daily using a fast discrete Fourier transformation.
The tide measurement file is a txt file whose name
corresponds to the name defined in the preprocessor
directive.  It is a series of NUMBER_OF_READINGS tidal
readings (in mm) taken twice per hour.
Author:			  Lachlan Reynolds
Student #s:		14511638
CS Accounts:	b2j3b
Date:		    September 14, 2022
*/

/******************************************************************
PLEASE EDIT THIS FILE

Sections of code in this file are missing.  You must fill them in.
Comments that start with // should be replaced with code
Comments that are surrounded by * are hints
******************************************************************/

/* Preprocessor directives */
#define _CRT_SECURE_NO_WARNINGS 
/*
  The preprocessor directive above allows the use of the fopen and sscanf
  in Visual Studio, rather than requiring fopen_s and sscanf_s.
  You may use fopen_s and sscanf_s while developing and debugging in Visual Studio,
  but please ensure that you replace these with fopen and sscanf prior to
  submitting your code to PrairieLearn.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tide_analyzer.h"
#include "discrete_fourier_transform.h"

#define FILE_HEIGHT         32744  /*Number of lines in the puddlejump.txt file that contain integers */
#define DIR                 1      /*Forward FFT according to code documentation */ 
#define NUMBER_OF_READINGS  131072 /* This = 2^17 (hint!) */
#define NOISE_FILTER        0.01   /* Used to eliminate noise from observation */
#define EXPONENT            17     /* Can be used for call to fft */
#define LINESIZE            128    /* Maximum line length in data file */
#define SAMPLING_FREQUENCY  48     /* Number of tidal readings taken day */
#define MIN_VALUES_PER_LINE 2      /* Min # of integers on each line of data file */
#define MAX_VALUES_PER_LINE 6      /* Max # of integers on each line of data file */
#define FFT_ALGOR_BUFFER    4      /* Prevents Run Time Check Failure #2

If you experience this error upon
closing your program, increase the size
of the readings array to
NUMBER_OF_READINGS + FFT_ALGOR_BUFFER */
#define FILE_NAME           "puddlejump.txt"
#define RESULT_FILE_NAME    "result.txt"

/*
Function contains all the steps of analysis.
PRE:       NULL (no pre-conditions)
POST:      NULL (no side-effects)
RETURN:    IF the program exits correctly THEN 0 ELSE 1
*/
void run_analysis()
{
  /* Variables */
  double readings[NUMBER_OF_READINGS];                  /* Copy the data file to this array */
  double complex_component[NUMBER_OF_READINGS];         /* Can be used for the complex results of fft */
  double omega[NUMBER_OF_READINGS];                     /* Tidal frequencies */
  double frequency = 0.0;                               /* Tides occur with this frequency the most... */
  double amplitude = 0.0;                               /* ...and that frequency occured this many times */
  int i = 0;                                            /* Helpful iterator variable */
  FILE* raw_data_ptr;                                   /* A "pointer" to the raw data file */
  FILE* results_ptr;                                    /*Pointer to results.txt file*/


                                            /* Starts by initializing the elements in the tidal frequency array so that each omega[i] equals
                                            i * SAMPLING_FREQUENCY / NUMBER_OF_READINGS.  The sampling frequency that we have to use when
                                            analyzing the discrete Fourier transform *has* to be the one that was used to measure the signal.
                                            There is only one correct value (in this case it's twice per hour, so SAMPLING_FREQUENCY = 48). Otherwise
                                            it will give us the wrong frequencies. Remember we're mixing ints and doubles here, and we want
                                            the results to be doubles, so you need to do some casting.
                                            
                                            // for ( i = 0...
                                            //	omega[i] = ...
                                            */
  for (i = 0; i < NUMBER_OF_READINGS; i++)
  {                                                                                 //initialize tiday frequency array       
      omega[i] = i * (double)SAMPLING_FREQUENCY / NUMBER_OF_READINGS;               //cast sampling frequency and number of readings, 
      complex_component[i] = 0.0;                                                   //set all complex parts to 0
  }

                                            /*
                                            Opens the file (a text file, not a binary file) for reading, and not writing,
                                            by invoking the fopen_s function with the correct parameters. 
                                            // fopen_s( ... ) (for Visual Studio)
                                            // file_pointer = fopen( ... ); (for PrairieLearn submission)*/

  //fopen_s(&raw_data_ptr, FILE_NAME, "r");   //For Visual Studio
  raw_data_ptr = fopen(FILE_NAME, "r");       // for PrairieLearn submission

                                            /* If the file pointer does not equal NULL THEN closes the pointer */
                                            // if (file_pointer != ...
  if (raw_data_ptr == NULL) {
      printf("Could not find raw data file.\n");
      return;
  }
                                                                                                                                                             /* Invokes the process file function, passing the the data readings array and the file pointer */
                                            // process_file(...
  process_file(readings, raw_data_ptr);


                                            /* Performs the Fourier transformation by passing the data readings, the complex result
                                            array, and two other parameters to the fft function. Since arrays are pass by reference
                                            and not pass by value, the function will be able to directly change the values in the
                                            cells.  Contrast this with the idea of pass by value, where we pass the value of a
                                            variable and if we change this value, the original variable remains unchanged. 
                                            // fft(...*/
  fft(DIR, EXPONENT, readings, complex_component);
                                            /* Since the Microsoft Visual Studio compiler doesn't support complex
                                            numbers without some tweaking, let's square the real and the complex components,
                                            add them, and take the square root.  Iterate through the elements of the readings
                                            array and change each readings[i] to equal (readings[i]^2 + complex_component[i]^2)^(1/2)
                                            
                                            // for (i = 0...
                                            // 	readings[i] = ...*/
  for (i = 0; i < NUMBER_OF_READINGS; i++) {
      readings[i] = sqrt((pow(readings[i], 2)+ pow(complex_component[i], 2)));
  }
                                            /*
                                            Searches through the results for the tidal frequency with the greatest amplitude.
                                            We needn't examine every value stored in the transformed readings array.  It is enough
                                            to look through the first NUMBER_OF_READINGS / 2.  Why?  The discrete Fourier transform
                                            does not accurately represent the Fourier coefficients for values of omega larger than
                                            NUMBER_OF_READINGS / 2 (it actually gives you the same as the first half, but in reverse
                                            order), and that is why they shouldn't be considered.

                                            So for each of the first NUMBER_OF_READINGS / 2 readings, make sure that the value in the
                                            frequency array, omega is >=  NOISE_FILTER.  If the frequency array value is greater than this
                                            minimum bound, then check if the value in the readings array is greater than any other so far.
                                            If it is, store both the frequency and the amplitude.
                                            */
                                            // for (i = 0...
  for (i = 0; i < NUMBER_OF_READINGS / 2; i++) {
      if (omega[i] < NOISE_FILTER){
          omega[i] = NOISE_FILTER;
      }

      else
      {
          if (readings[i] > amplitude)
          {
              frequency = omega[i];
              amplitude = readings[i];
          }
      }

  }
 
                                            /* You can use this for debugging, or (even better) you can set a breakpoint
                                            on this line, and look at the values of frequency and amplitude using
                                            the debugger
                                            */
  printf("Max Frequency = %f Max Amplitude = %f\n", frequency, amplitude);

                                            /* Creates (opens) a result file using fopen_s 
                                            // fopen_s ( ... ) (for Visual Studio)
                                            // file_pointer = fopen( ... ) (for PrairieLearn submission)*/

  //fopen_s(&results_ptr,  RESULT_FILE_NAME, "w");                      //for visual studio coding
  
  results_ptr = fopen(RESULT_FILE_NAME, "w");                         //for PrairieLearn submission
                                            /* Writes the result to the file */
                                            
  if (results_ptr == NULL) {
      printf("Could not find results file\n");
      return;
  }
  /* DO NOT MODIFY THIS LINE */
  fprintf(results_ptr, "Puddlejump tidal frequency: %f per day\n", frequency);

                                            /* Closes the result file */
                                            // if (file_pointer ...
  fclose(raw_data_ptr);
  fclose(results_ptr);

                                            /* And that's it */
  printf("Analysis complete, result.txt created\n");
}

/*
* Processes the data file
* PARAM:     array_to_populate is an array which is at least large enough
*            to contain the data in the specified file
* PARAM:     pointer_to_data_file is a valid and open file pointer
* PRE:       The file pointer is a reference to an open file
* PRE:       The file is a text file which contains between 2 and
*            6 real integers per line
* POST:      The contents of the file have been copied to the
*            specified array
* RETURN:    VOID
*/
void process_file(double array_to_populate[], FILE* pointer_to_data_file)
{
  /* Variable list */
  char  line_buffer[LINESIZE];
  int extracted_values[MAX_VALUES_PER_LINE];
  int readings_processed = 0;                                           //dont even need this variable but I dont want to delete it bc its good to keep track of readings processed for debugging
  int i = 0;
  int file_line_index = 0;                                              //used to increment th
  double temp_array[NUMBER_OF_READINGS];
  int values_per_line[FILE_HEIGHT];                                     //Stores the number of integers on each line to populate processed temp_array


  /* Copies the file, line by line, to line buffer using fgets in a while loop 
  // while( fgets (... ) ) {

  Tries to extract MAX_VALUES_PER_LINE ints from the line buffer and assign
  them to local array cells using sscanf_s or equivalent.  Stores the return
  value in a local int */
  // values_per_line = sscanf_s ( ... ) (for Visual Studio)
  // values_per_line = sscanf( ... ) (for PrairieLearn submission)

  while (fgets(line_buffer, LINESIZE, pointer_to_data_file) != NULL) {

      values_per_line[file_line_index] = str_to_arr(line_buffer, extracted_values);          //converts string of numbers to integer array and returns the number of integers stored in said array

      for (int j = 0; j < values_per_line[file_line_index]; j++) {
   
          temp_array[j + i] = (double)extracted_values[j];                                   //need a temp_array to store the values before populating array_to_populate (unsure why)
      }

      i+=values_per_line[file_line_index];                                                   //i acts as an index to skip over previously populated cells of temp array
      file_line_index++;
     
  }


  /* Copies the extracted integers to our data array.  Use a for loop for each
  of the values_per_line cells in the local array, and add the value
  stored in the cell to the end of the array we are populating with data value. */
  // for ( i = 0...

  for (i = 0; i < NUMBER_OF_READINGS; i++) {
      array_to_populate[i] = temp_array[i];
      readings_processed++;                                                                 
  }


  /* Keep track of what has been processed.  Increment the number of readings processed
  by the number of values successfully extracted from the line in the file. */
  // readings_processed +=

  /* End of function */
  return;
}

/*
* Prints the array
* PARAM:     array, the array to print
* PARAM:     lenght, the length of the array to avoid overshooting the boundary
* PRE:       populated array, otherwise printing garbage  
* POST:      NULL: Does not change values of the array
* RETURN:    VOID
*/
void print_array(int array[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

/*
* Converts the string of number characters in line_buffer to an array of integers and returns the number of values assigned
* PARAM:     char ptr to string
* PARAM:     arr, array to convert integers to
* PRE:       There exists a populated string to convert (populated by fgets(...))
* PRE:       An uninitialized array
* POST:      The contents of the string have been copied to the
*            specified array
* RETURN:    The number of integers copied
*/
int str_to_arr(char* string, int arr[]) {
    //Initialize Variables
    int num, i = 0, length;

    while (sscanf(string, "%d%n", &num, &length) == 1) {                  

        arr[i] = num;                                                       
        string += length;                                                   
        i++;                                                               
    }
    return i;                                                               
}   
