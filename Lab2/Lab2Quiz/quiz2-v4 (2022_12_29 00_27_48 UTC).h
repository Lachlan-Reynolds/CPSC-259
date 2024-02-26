/*
 File:           quiz2-v4.h
 Purpose:	       CPSC259 In-lab quiz 2
 */

#pragma once

/*
 * Determines the number of times the specified search string
 * appears in the specified DNA segment, and replaces each instance with the
 * specified replacement nucleotide.
 * PARAM: count, a pointer to an int
 * PARAM: segment, an array of char (a string)
 * PARAM: search_string, an array of char (a string)
 * PRE:   count is a pointer to an int
 * POST:  count stores the numbers of replacements made.
 * POST:  every 'A' (find_me) in the sequence has been replaced with a 'C' (replacement)
 */
void string_fun(int* count, char* segment, char find_me, char replacement);