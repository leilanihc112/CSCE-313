/* 
    File: bounded_buffer.C

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/11/04

    Bounded buffer program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include "bounded_buffer.H"

using namespace std;

BoundedBuffer::BoundedBuffer(int _size) : full(0), empty(_size) {}

BoundedBuffer::~BoundedBuffer() {}

void BoundedBuffer::Deposit(string _data) {
	empty.P();
	mutex.Lock();
	buffer.push_back(_data);
	mutex.Unlock();
	full.V();
}

string BoundedBuffer::Remove() {
	full.P();
	mutex.Lock();
	string returnData = buffer.front();
	buffer.pop_front();
	mutex.Unlock();
	empty.V();
	return returnData;
}