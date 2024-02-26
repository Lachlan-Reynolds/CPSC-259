/*
 File:         linkedlist.c
 Purpose:      Implements the linked list functions whose prototypes
               are declared in the linked_list.h header file
 Author:       Lachlan Reynolds
 Student #s:   14511638
 CWLs:         lreyno02
 Date:         OCtober 18th, 2022
 */

#define _CRT_SECURE_NO_WARNINGS

 /* Preprocessor directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

/******************************************************************
 PLEASE EDIT THIS FILE

 Comments that start with // should be replaced with code
 Comments that are surrounded by * are hints
 ******************************************************************/

 /*
  Returns a pointer to a new, empty linked list of struct node.
  PRE:       NULL (no pre-conditions)
  POST:      NULL (no side-effects)
  RETURN:    A pointer to a new, empty linked list of struct node (NULL)
  */
node* create_linked_list()
{

    node* new_node;
    new_node = NULL;
    return new_node;
}

/*
 Returns a dynamically allocated struct node that contains the specified
 struct airplane. Note that the string attributes of the newly created
 node's struct airplane must be specially copied (refer to lab 2).
 The node's next pointer doesn't point to anything
 PARAM:     plane, a struct airplane
 PRE:       NULL (no pre-conditions)
 POST:      NULL (no side-effects)
 RETURN:    A pointer to a dynamically allocated struct node a) that contains
            the specified struct airplane, and b) whose next pointer points to NULL
 */
node* create_node(airplane plane)
{

    struct node* new_node = NULL;
    new_node = (node*)malloc(sizeof(node));
    
    if (new_node == NULL) {
        printf("Unable to allocate memory\n");
        return NULL;
    }

    new_node->plane = plane;
    new_node->next = NULL;
    
    
    return new_node;
}





/*
 Prepends a struct node passed as a parameter to the list passed as a
 parameter, and returns a pointer to the result.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PARAM:     new_node is a pointer to a struct node
 PRE:       new_mode is not null
 POST:      The specified struct node is added to the beginning of the list
            and its next pointer points to the node that used to be first
 RETURN:    A pointer to a linked list which contains all of the nodes
            of the list passed as a parameter, plus a prepended node
 */
node* prepend_node(node* list, node* new_node)
{ 

    new_node->next = list;

  return new_node;
}

/*
//Create test node for figuring out weave function
test_node* create_test_node(int data)
{

    struct test_node* new_test_node = NULL;
    new_test_node = (test_node*)malloc(sizeof(test_node));

    if (new_test_node == NULL) {
        printf("Unable to allocate memory\n");
        return NULL;
    }

    new_test_node->data = data;
    new_test_node->next = NULL;


    return new_test_node;
}

//prepend test node for figuring out weave function
test_node* prepend_node(test_node* list, test_node* new_node)
{

    new_node->next = list;

    return new_node;
}

*/

/*
 Deletes the first struct node in the specified list, and returns a pointer to
 the shortened list. Be aware of nested allocations!
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PRE:       NULL
 POST:      The list passed as a parameter has one fewer node, and the dynamically
            allocated memory which contained that removed node has been freed
 RETURN:    IF the list is empty
            THEN NULL
            ELSE a pointer to the shortened list
 */
node* delete_node(node* list)
{

    if (list == NULL) {
        return NULL;
    }

    list = list->next;

    return list;
}

/*
 Returns the number of nodes in the specified linked list of struct node.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PRE:       NULL (no pre-conditions)
 POST:      NULL (no side-effects)
 RETURN:    The length of the specified list, an int
 */
int get_length(node* list)
{
    int length = 0;
    struct node* ptr = list;
    
    while (ptr != NULL) {
        ptr = ptr->next;
        length++;
    }

  return length;
}

/*
 Deletes an entire list.  This function iterates along a list and deallocates the
 memory containing each struct node.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PRE:       NULL (no pre-conditions)
 POST:      The memory containing all of the nodes in the list has been freed
 RETURN:    an empty list (NULL)
 */
node* delete_list(node* list)
{
    struct node* ptr = list;

    while (ptr != NULL) {
        list = list->next;
        free(ptr);
        ptr = list;
    }

  return ptr;
}

/*
 Prints the data stored in a node.  For the struct airplane in the node,
 prints the fields on separate lines.  For each field, prints the field name
 and the field value.  For the pointer, prints "Link = NULL\n" if the pointer points
 to null, else prints "Link points to address xxx\n", where xxx is the pointer address
 in hexademical.
 If node_to_print is NULL, prints "The node is empty\n".
 PARAM:     node_to_print is a pointer to a (possibly null) struct node
 PRE:       NULL (no pre-conditions)
 POST:      Information about node_to_print has been printed to standard output
 RETURN:    NULL
 */
void print_node(node* node_to_print)
{
    if (node_to_print == NULL) {
        printf("The node is empty\n");
        return;
    }

    printf("\n");
    printf("Flight number: %d\n", (*node_to_print).plane.flight_number);
    printf("City origin: %s\n", (*node_to_print).plane.city_origin);
    printf("City destination: %s\n", (*node_to_print).plane.city_destination);
    printf("Priority: %d\n", (*node_to_print).plane.priority);
    printf("Maximum speed in kph: %d\n", (*node_to_print).plane.maximum_speed_kph);
    printf("Cruising altitude: %d\n", (*node_to_print).plane.cruising_altitude);
    printf("Capacity: %d\n", (*node_to_print).plane.capacity);


    if (node_to_print->next == NULL) {
        printf("Link is NULL\n");
        return;
    }

    printf("Link = %h\n", &node_to_print->next);
    printf("\n");


    return;
}

/*
 Prints the data stored in all nodes in the specified list to print.
 For each node, prints the data inside the node the same way that it
 is printed by print_node.  If the list is empty, a suitable message
 is printed to standard output.
 PARAM:     node_to_print is a pointer to a (possibly empty) linked list of
            struct node
 PRE:       NULL (no pre-conditions)
 POST:      Information about list_to_print has been printed to standard output
 RETURN:    NULL
 */
void print_list(node* list_to_print)
{
    int i = 1;
    if (list_to_print == NULL) {
        printf("\n");
        printf("List is NULL\n");
        return;
    }

    while (list_to_print != NULL) {
        printf("Node: %d\n", i);
        print_node(list_to_print);
        list_to_print = list_to_print->next;
        i++;
    }
    return;
}

/*
 Reverses a list.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PRE:       NULL (no pre-conditions)
 POST:      The order of the nodes in the list passed as a parameter has been
            reversed
 RETURN:    a list of struct node that is the reverse of the list passed as a
            parameter
 */
node* reverse(node* list)
{
    struct node* prev=NULL;
    struct node* current=list;
    struct node* next=NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list = prev;

  return list;
}

/*
 Removes every node from the list which contains an airplane destined for
 the city name passed in the second parameter.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PARAM:     destination_city is a pointer to a null-terminated array of char
 PRE:       NULL (no pre-conditions)
 POST:      Any struct node in the list which contains an airplane struct
            destined for the destination_city is removed from the list, and
            the memory deallocated
 RETURN:    a list of struct node that does not contain any struct node that
            has an airplane destined for the destination_city
 */
node* remove_from_list(node* list, char* destination_city)
{
    //Check for empty list
    if (list == NULL) {
        return NULL;
    }

    struct node* current = list;                //Pointing to node we might want to delete
    struct node* prev=list;                     //Pointing to node before that
    struct airplane current_data = current->plane;
    char* check_condition = current_data.city_destination;
    int i = 0;

    while (current != NULL) {
        current_data = current->plane;
        check_condition = current_data.city_destination;

        if (strcmp(check_condition, destination_city) == 0) {
            
            if (i ==0) {
                list = current->next;
            }

            prev->next = current->next;
            free(current);
            current = NULL;
            
        }

        else {
            prev = current;
            current = current->next;                //iterate to next node
            
        }
        i++;
    }

  return list;
}

/*
 Returns a reference to the nth node (but does not remove it ) in the
 specified list.  If the list is too short, returns NULL.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PARAM:     ordinality is an integer
 PRE:       ordinality > 0
 POST:      NULL (no side-effects)
 RETURN:    IF ordinality <= length of list
            THEN a pointer to the nth node
            ELSE NULL
 */
node* retrieve_nth(node* list, int ordinality)
{
    struct node* ptr = list;
    struct node* temp;
    int count = 1;

    if (ordinality > get_length(list)) {
        return NULL;
    }

    while (ptr != NULL) {


        if (count == ordinality) {
            return ptr;
        }

        count++;
        ptr = ptr->next;
    }

    return NULL;
}

/*
 Inserts the specified node into the specified list of nodes at the specified
 ordinality.  For example, if ordinality = 1, this is a simple prepend_node
 operation.  If ordinality = 3, then when this function is finished, the third
 node in the list will be the node_to_insert.  If the ordinality = the length
 of the list + 1, then the node_to_insert is appended to the end of the list.
 If the ordinality > 1 + length of the list, the function returns a pointer
 to the unchanged list.
 PARAM:     list is a pointer to a (possibly empty) linked list of struct node
 PARAM:     ordinality is an integer
 PRE:       ordinality > 0
 PRE:       node_to_insert is NEVER a null (it is always a correct node pointer)
 POST:      NULL (no side-effects)
 RETURN:    IF ordinality <= length of list + 1
            THEN a pointer to the list which contains the node_to_insert in the
      correct location
            ELSE a pointer to the unchanged list
 */
node* insert_nth(node* list, node* node_to_insert, int ordinality)
{
    int count = 1;
    struct node* return_val = prepend_node(list, node_to_insert);
    struct node* ptr = list;
    struct node* temp = node_to_insert;

    if (ordinality > get_length(list)+1) {               //Check if ordinality is too large
        return list;
    }
    
    if (list == NULL||ordinality==1) {                     //Empty List prepends the list
        return return_val;
    }

   
    while (ptr != NULL) {
       
        if (count == ordinality -1) {           //not the second last case (need to implement

            temp->next = ptr->next;
            ptr->next = temp;
            return list;

        }

        if (ptr->next == NULL && count!=ordinality) {            //append to list (ordinality=length+1)
            temp->next = NULL;
            ptr->next = temp;
            return list;
        }


        
        
        count++;
        ptr = ptr->next;
        
    }



}