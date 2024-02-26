/*
 File:         main.c
 Purpose:      Contains the main function for driving the program.
               You may add your own testing code here in the main
               function, but this file will not be submitted for
               credit.
 Author:       Lachlan Reynolds
 Student #s:   14511638
 CWLs:         lreyno02
 Date:         October 18th, 2022
 */

 /* Preprocessor directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

/*
 Main function drives the program.  Every C program must have one
 main function.  A project will not compile without one.

 In our program, the main function does nothing.  That's because
 our program doesn't do anything.  We're just implementing a linked
 list and testing it using our unit tests.

 You may add your own code for testing if you wish.

 PRE:    NULL (no pre-conditions)
 POST:	 NULL (no side-effects)
 RETURN: IF the program exits correctly
     THEN 0 ELSE 1
 */


int main(void)
{
   
    airplane test_airplane_one = { 100, "Vancouver", "Toronto", 1, 900, 36000, 350 };
    airplane test_airplane_two = { 200, "Toronto", "Halifax", 2, 800, 26000, 280 };
    airplane test_airplane_three = { 300, "Calgary", "Edmonton", 3, 700, 30000, 200 };
    airplane test_airplane_four = { 400, "Kelowna", "Prince George", 4, 600, 20000, 100 };
    node* test_node_one = create_node(test_airplane_one);
    node* test_node_two = create_node(test_airplane_two);
    node* test_node_three = create_node(test_airplane_three);
    node* test_node_four = create_node(test_airplane_four);
    node* test_list = create_linked_list();
    test_list = prepend_node(test_list, test_node_one);
    test_list = prepend_node(test_list, test_node_two);
    test_list = prepend_node(test_list, test_node_three);
    
    node* head1 = test_list;
    node** head2 = &test_list;
    

    print_list(head1);
    print_list(*head2);
    head1 = Weave(head1, head2);

    print_list(head1);
	





  /* The system command forces the system to pause */
  system("pause");
  return 0;
}

/*
 *  RECURSIVELY swaps the positions of each pair of adjacent nodes.
 *  This must be achieved by reassigning/relinking the next attribute
 *  of the list nodes. You may NOT overwrite the data attribute of
 *  existing nodes and you may NOT malloc or free existing nodes.
 *
 *  You may assume that the list length is a multiple of 2.
 *
 *  THIS MUST BE ACHIEVED RECURSIVELY TO EARN FULL CREDIT.
 *  THERE WILL BE A MANUALLY-GRADED COMPONENT WORTH APPROXIMATELY
 *  50% CREDIT FOR THIS PROGRAMMING QUESTION. A RECURSIVE SOLUTION
 *  (CORRECT OR OTHERWISE) WILL AUTOMATICALLY EARN THE MANUAL COMPONENT.
 *
 *  Example:
 *  before:  A - B - C - D - E - F
 *  after:   B - A - D - C - F - E
 *
 *  @param list - pointer to the first node in a linked list to be rearranged.
 *  @return - pointer to the front of the rearranged list.
 */
struct node* Rearrange(struct node* list) {
    
    if (list == NULL) {
        return NULL;                //checks for empty list
    }

    struct node* ptr = list;
    struct node* temp = ptr->next;

    if (temp->next == NULL) {               //base case
        temp->next = ptr;                   //temp is pointing to null therefore it is last, make it point to second last
        ptr->next = NULL;                   //make second last point to null
        return temp;                
    }
    
    else {
        ptr = Rearrange(temp->next);
        return list;
    }

    
}















/*
 *  Weaves the contents of two null-terminated, singly-linked lists with
 *  head pointers.
 *  This must be achieved by reassigning/relinking the next attribute
 *  of the list nodes. You may NOT overwrite the data attribute of
 *  existing nodes and you may NOT malloc or free existing nodes.
 *
 *  Note that list2 is a double-pointer type.
 *  Think of this as a struct node* passed by reference (we want to modify
 *  the actual parameter).
 *
 *  @param list1 - pointer to the first node in a linked list to be woven.
 *  @param list2 - reference to the actual pointer to another list to be woven.
 *                 Will be modified by this procedure.
 *  @return - pointer to the front of the woven list.
 */
struct node* Weave(struct node* list1, struct node** list2) {
    
    struct node* ptr1 = list1;              //initialize at head
    struct node* ptr2 = *list2;
    struct node* temp1 = ptr1->next;        //next node in list 1
    struct node* temp2 = ptr2->next;        //next node in list 2

    while (ptr1!=NULL && ptr2!=NULL) {

        ptr1->next = ptr2;

        if (temp1 != NULL) {

            ptr2->next = temp1;
        }
        
        else {
            break;
        }

        ptr1 = temp1;
        ptr2 = temp2;

        if (temp1 != NULL) {
            temp1 = temp1->next;
        }

        if (temp2 != NULL) {
            temp2 = temp2->next;
        }
    }


    ptr1 = NULL;
    ptr2 = NULL;
    temp1 = NULL;
    temp2 = NULL;
    *list2 = NULL;
    return list1;
}
