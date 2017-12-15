/* 
    File: my_allocator.c

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 09/18/17

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".
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
#include "my_allocator.h"

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
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

    char* st;
  
unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length) {
  /* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. */ 
   
   	st = (char*)malloc((int)sizeof(FL_HEADER) + _length);
   
    flist = (FL_HEADER*)st;
	flist -> length = _length;
	flist -> next = NULL;
	flist -> prev = NULL;
	
	if (st == 0 || _length == 0 || _basic_block_size > _length || _basic_block_size == 0)
	 	return 0;
	else
		return _length;
}
				
Addr my_malloc(size_t _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation. */
  /* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. 
   NOTE: In the old days one used 'unsigned int' for the length. The C
   Standards Committee has added the 'size_t' data type to ensure to 
   represent memory sizes. This makes the code more portable. Depending
   on the architecture, 'size_t' may or may not be the same 
   as 'unsigned int'. */ 
   
   FL_HEADER* FL_temp = flist; // temp start to free_list
   
   while (FL_temp -> next != NULL) {
	   if (FL_temp -> length >= _length) {
		   FL_temp = FL_temp -> next; 
		   break;
	   }
	   else {
		   FL_temp = FL_temp -> next; 
		   FL_temp = NULL;
	   }
   }
   
   if (FL_temp == NULL) // out of memory
	   return 0;
   else {
	   FL_temp -> length = _length;
	   FL_HEADER* FL_temp2 = (FL_HEADER*)malloc((int)sizeof(FL_HEADER) + ((FL_temp -> length) - _length));
	   FL_add(flist,FL_temp2);
	   FL_remove(flist,FL_temp);
	   return FL_temp + sizeof(FL_temp);
   }
}

int release_allocator() {
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails. */ 
}

int my_free(Addr _a) {
  /* Same here! */
  /* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 
   FL_HEADER* fl_free = (FL_HEADER*)_a;
   FL_add(flist,fl_free);
   return 0;
}