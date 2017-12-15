/* 
    File: semaphore.C

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/10/15

    Semaphore program for MP2 in CSCE 313
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

#include "semaphore.H"

using namespace std;

  Semaphore::Semaphore(int _val) : value(_val) {
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
  }

  Semaphore::~Semaphore() {
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
  }

  int Semaphore::P() {
	pthread_mutex_lock(&m);
	value--;
	if (value < 0) {
		pthread_cond_wait(&c, &m);
	}
		pthread_mutex_unlock(&m);
	return value;
  } 

  int Semaphore::V() {
	pthread_mutex_lock(&m);
	value++;
	if (value <= 0) {
		pthread_cond_signal(&c);
	}
	pthread_mutex_unlock(&m);
	return value;
  }