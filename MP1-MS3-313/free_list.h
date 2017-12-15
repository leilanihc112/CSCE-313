/* 
    File: free_list.h

    Author: R. Bettati
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 09/18/17

    Modified:

*/

#ifndef _free_list_h_                   // include file only once
#define _free_list_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr;

typedef struct fl_header { /* header for block in free list */
	int length;
	int free;
	struct fl_header* next;
	struct fl_header* prev;
} FL_HEADER; /* I don’t like to type ’struct’ all the time, so */
			 /* I like to typedef it away... */
			 
extern FL_HEADER* flist; // start of free_list
			 
void FL_remove(FL_HEADER * free_list, Addr * startNode);
/* Remove the given block from given free list. The free-list
   pointer points to the first block in the free list. Depending
   on your implementation you may not need the free-list pointer.*/
   
void FL_add(FL_HEADER * free_list, Addr * startNode);
/* Add a block to the free list. */

#endif 