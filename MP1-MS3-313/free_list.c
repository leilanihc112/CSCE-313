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

void FL_remove(FL_HEADER * free_list, Addr* startNode) {
/* Remove the given block from given free list. The free-list
   pointer points to the first block in the free list. Depending
   on your implementation you may not need the free-list pointer.*/
   free_list -> free = 0;
   
   if(free_list -> prev == NULL && free_list -> next == NULL) { // if there is only one node
		*(startNode) = NULL;
   }
   else if (free_list -> next == NULL) { // last block
		free_list -> prev -> next = NULL;
   }
   else if (free_list -> prev == NULL) {  // first block
		*(startNode) = (Addr)(free_list -> next);
		free_list -> next -> prev = NULL;
   }
   else { // middle block
		free_list -> next -> prev = free_list -> prev;
		free_list -> prev -> next = free_list -> prev;
   }
}

void FL_add(FL_HEADER * free_list, Addr * startNode) {
	/* Add a block to the free list. */
	free_list -> free = 1;
	free_list -> next = (FL_HEADER*)(*(startNode));
	free_list -> prev = NULL;
	 if (*(startNode) != NULL) // if free_list is empty
		((FL_HEADER*)*(startNode)) -> prev = free_list;
	 *(startNode) = (Addr)free_list;
}

Addr FL_INIT (int _length) {
    Addr flist = (Addr)malloc(_length);
	((FL_HEADER*)flist) -> length = _length;
	((FL_HEADER*)flist) -> next = NULL;
	((FL_HEADER*)flist) -> prev = NULL;
	((FL_HEADER*)flist) -> free = 1;
	return flist;
}