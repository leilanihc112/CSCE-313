/* 
    File: mutex_guard.C

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/10/15

    Mutex guard program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include "mutex_guard.H"

using namespace std;

  MutexGuard::MutexGuard(Mutex & m) {
	  m.Lock();
	  this->m = &m;
  }

  MutexGuard::~MutexGuard() {
	  m.Unlock();
  }