/* 
    File: bounded_buffer.H

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/11/04

*/

#ifndef _bounded_buffer_H_                   // include file only once
#define _bounded_buffer_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
#include <list>
#include <string>
#include "semaphore.H"
#include "mutex.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   B o u n d e d B u f f e r  */
/*--------------------------------------------------------------------------*/

class BoundedBuffer {
private:
	std::list<std::string> buffer;
	Semaphore full;
	Semaphore empty;
	Mutex mutex;
	
public:
	    /* -- CONSTRUCTOR/DESTRUCTOR */
    BoundedBuffer(int _size);
    ~BoundedBuffer();
    /* -- BUFFER OPERATIONS */
    void Deposit(std::string _data);
    std::string Remove();
};
#endif