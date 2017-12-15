/* 
    File: client.C

    Author: Leilani Horlander-Cruz
            Department of Computer Science
            Texas A&M University
    Date  : 2017/10/15

    Client main program for MP2 in CSCE 313
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
#include <sys/time.h>

#include "reqchannel.H"

using namespace std;

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
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

  double totalReqTime = 0.0;
  int nRequests = 0; 
  
string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  pid_t c_pid = fork(); 
  if (c_pid == 0) // create new child process 
	  execv("./dataserver",NULL); // run data server
  cout << "CLIENT STARTED:" << endl;

  cout << "Establishing control channel... " << flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  cout << "done." << endl;

  /* -- Start sending a sequence of requests */

 /* string reply1 = chan.send_request("hello");
  cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

  string reply2 = chan.send_request("data Joe Smith");
  cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

  string reply3 = chan.send_request("data Jane Smith");
  cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl; */

  for(int i = 0; i < 100; i++) {
    string request_string("data TestPerson" + int2string(i));
	
	struct timeval start, end;
	//clock_t start = clock();
	gettimeofday(&start, NULL);
	//cout << (double)start << endl;
    string reply_string = chan.send_request(request_string);
	gettimeofday(&end, NULL);
	double elapsedReq = end.tv_usec - start.tv_usec;
	//clock_t end = clock();
	//cout << (double)end << endl;
	//double elapsedReq = (double)(end-start)/CLOCKS_PER_SEC;
    totalReqTime += elapsedReq;
	nRequests++;

	cout << "reply to request " << i << ":" << reply_string << endl;
   }
  string reply4 = chan.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;
  
	double avgReqTime = totalReqTime/nRequests;
	//cout << "avg send_request request time for " << nRequests << " requests is " << avgReqTime*1e6 << " microseconds\n";
    printf("avg send_request request time for %d requests is %f microseconds\n", nRequests, avgReqTime);
  usleep(1000000);
}