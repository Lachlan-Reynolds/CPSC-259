/*
 File:         linkedlist.h
 Purpose:      Prototypes for a linked list implementation
 Author:       Lachlan Reynolds
 Student #s:   14511638
 CWLs:         lreyno02
 Date:         OCtober 18th, 2022
 */

#pragma once

 /******************************************************************
	PLEASE EDIT THIS FILE

	Comments that start with // should be replaced with code
	Comments that are surrounded by * are hints
	******************************************************************/

	/* Insert your airplane structure here.  Use the correct types and names for the elements! */
typedef struct airplane {
	int flight_number;
	char* city_origin;
	char* city_destination;
	int priority;
	int maximum_speed_kph;
	int cruising_altitude;
	int capacity;
} airplane;

/* Insert your node structure here.  Use the correct types and names for the elements! */
typedef struct node {
	struct airplane plane;			//data
	struct node* next;				//pointer to node
} node;

typedef struct test_node {
	int data;
	struct test_node* next;
} test_node;



/* Prototypes for in-lab functions (do not add anything here) */
node* create_linked_list();
node* create_node(airplane plane);
node* prepend_node(node* list, node* new_node);
node* delete_node(node* list);

/* Prototypes for take-home functions (do not add anything here) */
int get_length(node* list);
node* delete_list(node* list);
void print_node(node* node_to_print);
void print_list(node* list_to_print);
node* reverse(node* list);
node* remove_from_list(node* list, char* destination_city);
node* retrieve_nth(node* list, int ordinality);
node* insert_nth(node* list, node* node_to_insert, int ordinality);


