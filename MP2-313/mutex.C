/* 
    File: mutex.C

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/10/15

    Mutex program for MP2 in CSCE 313
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

#include "mutex.H"

using namespace std;

  Mutex::Mutex() {
	  pthread_mutex_init(&m, NULL);
  }

  Mutex::~Mutex() {
	  pthread_mutex_destroy(&m);
  }
  
 void Mutex::Lock() {
	return pthread_mutex_lock(&m);
  }
  
  void Mutex::Unlock() {
	return pthread_mutex_unlock(&m);
  }