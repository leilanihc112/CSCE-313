/* 
    File: free_list.c

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 09/18/17

    Modified: 

    This file contains the implementation of the module "FREE_LIST".
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "free_list.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/
	
	/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE FREE_LIST */
/*--------------------------------------------------------------------------*/

FL_HEADER* flist; // start of free_list

void FL_remove(FL_HEADER * free_list, FL_HEADER * block) {
/* Remove the given block from given free list. The free-list
   pointer points to the first block in the free list. Depending
   on your implementation you may not need the free-list pointer.*/
   
   if (free_list == NULL || block == NULL) { // empty
		// there is some sort of error
   }
   else if(free_list -> prev == NULL && free_list -> next == NULL) { // if there is only one node
		// can't do
   }
   else if (block -> next == NULL) { // last block
		FL_HEADER* previous = block -> prev;
		previous -> next = NULL;
   }
   else if (block -> prev == NULL) {  // first block
		FL_HEADER* _next = block -> next;
		_next -> prev = NULL;
   }
   else { // middle block
	    FL_HEADER* previous = block -> prev;
		FL_HEADER* _next = block -> next;
		previous -> next = block -> next;
		_next -> prev = block -> prev;
   }
}

void FL_add(FL_HEADER * free_list, FL_HEADER * block) {
	/* Add a block to the free list. */
	
	 if (free_list == NULL) { // if free_list is empty
		free_list = block;
		free_list -> length = block -> length;
		free_list -> next = NULL;
	 	free_list -> prev = NULL;
	 }
	 else { // otherwise, insert at front
		 free_list -> prev = block; 
		 block -> next = free_list;
		 block -> prev = NULL;
		 free_list = block;
	 }
}