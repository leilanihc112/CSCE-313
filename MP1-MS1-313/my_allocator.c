/* 
    File: my_allocator.c

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 9-10-17

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
 unsigned int rem;
  
unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length) {
  /* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
  */ 
	st = (char*)malloc(_length);
	rem = _length;
	if (st == 0)
		return 0;
	else
		return _length;
}
				
Addr my_malloc(size_t _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  /* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. 
   NOTE: In the old days one used 'unsigned int' for the length. The C
   Standards Committee has added the 'size_t' data type to ensure to 
   represent memory sizes. This makes the code more portable. Depending
   on the architecture, 'size_t' may or may not be the same 
   as 'unsigned int'. 
  */ 
  if (_length > rem)
  {
	  return 0;
  }
  else
  {
	char* temp = st;
	st += _length;
	rem -= _length;
	return temp;
  }
}

int release_allocator(); 
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 

int my_free(Addr _a) {
  /* Same here! */
  free(_a);
  return 0;
}

