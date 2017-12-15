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

#define FL_size sizeof(FL_HEADER)

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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

    // char* st;
	static Addr *arrayAddr;
	static Addr current;
	static Addr addrSt;
	static int ind;
	static int arraySize;
	static int offset;
	
// return block in free list in segments of 2^n = total bits - header (use floor)
// basic_block_size is initial block size. Powers of 2
// coalescing small blocks into larger free blocks -> until get original size block
// buddy allocator
// for buddies -> only 1 bit difference -> need to check which is different
// size or diff is 2^k -> flip kth bit
// when splitting block size 2^k with start address A, two buddies created - start address A, and another with start address A with (k-1)th bit flipped
  
unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length) {
  /* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. */ 
   
   /*
   	st = (char*)malloc((int)sizeof(FL_HEADER) + _length);
	
	if (st == 0 || _length == 0 || _basic_block_size > _length || _basic_block_size == 0)
	 	return 0;
	else
		return _length;
	*/
	
	int up = (int)log2(_length); // powers of 2
	int low = (int)log2(_basic_block_size); // powers of 2
	offset = low;
	arraySize = up - low + 1;
	arrayAddr = (Addr*)malloc(arraySize *FL_size);
	
	int to_alloc = (int)pow(2.0, (double)up); // powers of 2
	for (int i = arraySize - 1; i >= 0; i--)
		arrayAddr[i] = NULL;
	current = FL_INIT(to_alloc);
	addrSt = current;
	arrayAddr[arraySize - 1] = current;
	current = arrayAddr[0];
	return 1;
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
   
   int power = ceil(log2(_length + FL_size));
   int arrayEnt = power - offset;
   if (arrayEnt < 0) {
	   arrayEnt = 0;
   }
   current = arrayAddr[arrayEnt];
   
   if (current != NULL) {
	   Addr rAddr = (Addr)((char*)current + FL_size);
	   FL_remove((FL_HEADER*)current, &arrayAddr[arrayEnt]);
	   return rAddr;
   }
   else {
	   ind = 0;
	   split_block(arrayEnt);
	   current = arrayAddr[arrayEnt];
	   if (current == NULL) 
		   return NULL;
	   else {
		   Addr rAddr = (Addr)((char*)current + FL_size);
		   FL_remove((FL_HEADER*)current, &arrayAddr[arrayEnt]);
		   return rAddr;
	   }
   }
   /*
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
   */
}

void split_block(int arrayEnt) {   // break down blocks into smaller blocks
	if (ind + arrayEnt != arraySize) {
		current = arrayAddr[ind + arrayEnt];
		if (arrayAddr[ind + arrayEnt] == NULL) {
			ind++;
			split_block(arrayEnt);
		}
		if (arrayAddr[ind + arrayEnt] != NULL && ind + arrayEnt > 0) {
			current = arrayAddr[ind + arrayEnt];
			int newLength = ((FL_HEADER*)current) -> length / 2;
			Addr new_n = (Addr)((char*)current + newLength);
			((FL_HEADER*)new_n) -> length = newLength;
			((FL_HEADER*)current) -> length = newLength;
			FL_remove((FL_HEADER*)current, &arrayAddr[arrayEnt + ind]);
			FL_add((FL_HEADER*)new_n, &arrayAddr[arrayEnt + ind - 1]);
			FL_add((FL_HEADER*)current, &arrayAddr[arrayEnt + ind - 1]);
			ind--;
		}
	}
}

void checkBuddy(Addr node) {  // check if buddies
	ind  = ceil(log2(((FL_HEADER*)node) -> length)) - offset;
	long int buddyBit = (long int)((char*)node - (char*)addrSt) ^ ((FL_HEADER*)node) -> length;
	Addr buddy = (Addr)(buddyBit + (char*)addrSt);
	if (node != addrSt && ind != arraySize - 1) {	
		if (((FL_HEADER*)buddy) -> free == 1 && ((FL_HEADER*)buddy) -> length == ((FL_HEADER*)node) -> length) {
			if(node < buddy)  {
				((FL_HEADER*)node) -> length *= 2;
				FL_remove((FL_HEADER*)buddy, &arrayAddr[ind]);
				FL_remove((FL_HEADER*)node, &arrayAddr[ind]);
				FL_add((FL_HEADER*)node, &arrayAddr[ind + 1]);
				checkBuddy(node);
			}
			else {
				((FL_HEADER*)buddy)->length *= 2;
				FL_remove((FL_HEADER*)buddy, &arrayAddr[ind]);
				FL_remove((FL_HEADER*)node, &arrayAddr[ind]);
				FL_add((FL_HEADER*)buddy, &arrayAddr[ind + 1]);
				checkBuddy(buddy);
			}
		}
	}
	ind = 0;
}

int release_allocator() {
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails. */ 
}

int my_free(Addr _a) {
  /* Same here! */
  /* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 
   _a = (Addr)((char*)_a - FL_size);
   ind = (int)log2(((FL_HEADER*)_a)->length);
   FL_add((FL_HEADER*)_a, &arrayAddr[ind - offset]);
   checkBuddy(_a);
}