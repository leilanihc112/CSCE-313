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
#include <vector>
#include <map>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#include "reqchannel.H"
#include "bounded_buffer.H"

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

struct reqArgs {
	int _numReqs;
	string _name;
	BoundedBuffer* _reqBuffer;
	reqArgs(string name, int numReqs, BoundedBuffer* reqBuffer) : _name(name), _numReqs(numReqs), _reqBuffer(reqBuffer) {}
};

struct eventHandlerArgs {
	BoundedBuffer* _workBuffer;
	map<string, BoundedBuffer*>* statBufferMap;
	vector<RequestChannel*>* channels;
	
	eventHandlerArgs(BoundedBuffer* workBuffer, vector<RequestChannel*>* channels, map<string, BoundedBuffer*>* statBufferMap) : _workBuffer(workBuffer), channels(channels), statBufferMap(statBufferMap) {}
};

struct statsArgs {
	string _name;
	BoundedBuffer* _statBuffer;
	vector<int>* histVec;
	
	statsArgs(string name, BoundedBuffer* statBuffer, vector<int>* histVec) : _name(name), _statBuffer(statBuffer), histVec(histVec) {}
};

//print histogram in the terminal
void printHist(vector<int>* hist) {
	cout << "histogram:" << endl;
	for(int i = 0; i < hist->size(); i++) {
		cout << "data val " << i << "(" << hist->at(i) << "): ";
		for (int j = 0; j < hist->at(i); j++) {
			cout << "x";
		}
		cout << endl;
	}
}

//build xls file for histogram
void buildHistFile(ofstream& fileData, vector<int>* hist) {
	// "\t" new column, "\r" new row
	fileData << "Data" << "\t" << "Count" << "\r";
	for (int i = 0; i < hist->size(); i++) {
		fileData << i << "\t" << hist->at(i) << "\r";
	}	
}

bool exData = false;
bool printHistogram = false;
int numReqs = 1000;   //# requests to issue per person
int reqBufferSize = 100;  //size of request buffer
int numReqChannels = 1;   //# of active worker threads
int activeReqThreads = 3;  //counter -> how many req threads active
int activeWorkThreads;     //counter -> how many worker threads active

Mutex reqCounterLock;
Mutex workCounterLock;

  //double totalReqTime = 0.0;
  //int nRequests = 0; 
  
string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

void* reqFunc(void* argsStr) {
	reqArgs* arg = (reqArgs*)argsStr;
	int numReqs = arg->_numReqs;
	string name = arg->_name;
	BoundedBuffer* reqBuffer = arg->_reqBuffer;
	
	for (int i = 0; i < numReqs; i++) {
		reqBuffer->Deposit("data " + name);
	}
	
	reqBuffer->Deposit("done");
}

void* eventHandlerFunc(void* argsStr) {
	eventHandlerArgs* arg = (eventHandlerArgs*)argsStr;
	BoundedBuffer* workBuffer = arg->_workBuffer;
	map<string, BoundedBuffer*>* statBufferMap = arg->statBufferMap;
	vector<RequestChannel*>* channels = arg->channels;
	
	map<int, RequestChannel*> rcMap;
	map<int, string> nameMap;
	
	for (int i = 0; i < channels->size(); i++) {
		string req = workBuffer->Remove();
		string name = req.substr(5);
		channels->at(i)->cwrite(req);
		
		rcMap.insert(pair<int, RequestChannel*>(channels->at(i)->read_fd(), channels->at(i)));
		
		nameMap.insert(pair<int, string>(channels->at(i)->read_fd(), name));
	}
	
	fd_set rSet;
	
		
	for (;;) {
		FD_ZERO(&rSet);
		int maxfd = 0;
		
		for (int i = 0; i < channels->size(); i++) {
			FD_SET(channels->at(i)->read_fd(), &rSet);
			if (channels->at(i)->read_fd() > maxfd) {
				maxfd = channels->at(i)->read_fd();
			}
		}
		
		select(maxfd+1, &rSet, NULL, NULL, NULL);
		
		for (int j = 0; j < maxfd; j++) {
			if (FD_ISSET(j, &rSet)) {
				RequestChannel* cReply = rcMap.find(j)->second;
				string reply = cReply->cread();
				string name = nameMap.find(j)->second;
				map<string, BoundedBuffer*>::iterator statMapIt = statBufferMap->find(name);
				string newReq = workBuffer->Remove();
				
				if(newReq == "done") {
					reqCounterLock.Lock();
					activeReqThreads--;
					reqCounterLock.Unlock();
			
					if (activeReqThreads == 0) {
						for(int i = 0; i < numReqChannels; i++) {
							workBuffer->Deposit("quit");
						}
					}
				}
				else if (newReq == "quit") {
					workCounterLock.Lock();
					activeWorkThreads--;
					workCounterLock.Unlock();
			
					if(activeWorkThreads == 0) {
						for (auto it = statBufferMap->begin(); it != statBufferMap->end(); it++) {
							it->second->Deposit("quit");
						}
						break;
					}		
				}
				else {
					string newName = newReq.substr(5);
					cReply->cwrite(newReq);
					map<int, string>::iterator nameMapIt = nameMap.find(j);
					nameMapIt->second = newName;
				}
			}
		}
	}
}

void* statsFunc(void* argsStr) {
	statsArgs* arg = (statsArgs*)argsStr;
	string name = arg->_name;
	BoundedBuffer* statBuffer = arg->_statBuffer;
	vector<int>* histVec = arg->histVec;
	
	for(;;) {
		string data = statBuffer->Remove();
		if (data == "quit") {
			break;
		}
		else {
			try {
				histVec->at(stoi(data))++;
			}
			catch (invalid_argument& e) { //error checking
				continue;
			}
		}
	}
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
// parse command line arguments
	if (argc < 7)
	{ 
		cout << "Usage:" << endl;
		cout << "client -n <number of data requests per person>\n\t-b <size of bounded buffer between request and worker threads>\n\t-w <number of worker threads>";
		return 0;
    }
else {
	cout << "Parameters" << endl;
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-n") {
			numReqs = stoi(argv[i+1]);
			cout << "num data requests: " << stoi(argv[i+1]) << endl;
		}
		else if (string(argv[i]) == "-b") {
			reqBufferSize = stoi(argv[i+1]);
			cout << "buffer size: " << stoi(argv[i+1]) << endl;
		}
		else if (string(argv[i]) == "-w") {
			numReqChannels = stoi(argv[i+1]);
			activeWorkThreads = numReqChannels;
			cout << "num worker threads: " << stoi(argv[i+1]) << endl;
		}
		else if (string(argv[i]) == "-x") {
			exData = true;
			cout << "export data as .xls: true" << endl;
		}
		else if (string(argv[i]) == "-p") {
			printHistogram = true;
			cout << "print histograms: true" << endl;
		}
		else {
			continue;
		}
	}
	cout << endl << endl;
}

	pid_t c_pid = fork(); 
  if (c_pid == 0) // create new child process 
	  execv("./dataserver",NULL); // run data server
  cout << "CLIENT STARTED:" << endl;

  cout << "Establishing control channel... " << flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  cout << "done." << endl;
  
  BoundedBuffer* reqBuffer = new BoundedBuffer(reqBufferSize);
  
  reqArgs* reqsJane = new reqArgs("Jane Doe", numReqs, reqBuffer);
  reqArgs* reqsJoe = new reqArgs("Joe Smith", numReqs, reqBuffer);
  reqArgs* reqsJohn = new reqArgs("John Doe", numReqs, reqBuffer);
  
  //capacity of 100
  BoundedBuffer* statBufferJane = new BoundedBuffer(100);
  BoundedBuffer* statBufferJoe = new BoundedBuffer(100);
  BoundedBuffer* statBufferJohn = new BoundedBuffer(100);
  
  vector<int>* histJane = new vector<int>(100);
  vector<int>* histJoe = new vector<int>(100);
  vector<int>* histJohn = new vector<int>(100);
  
  statsArgs* statsJane = new statsArgs("Jane Doe", statBufferJane, histJane);
  statsArgs* statsJoe = new statsArgs("Joe Smith", statBufferJoe, histJoe);
  statsArgs* statsJohn = new statsArgs("John Doe", statBufferJohn, histJohn);
  
  map<string, BoundedBuffer*>* statsMap = new map<string, BoundedBuffer*>();
  
  statsMap->insert(pair<string, BoundedBuffer*>("Jane Doe", statBufferJane));
  statsMap->insert(pair<string, BoundedBuffer*>("Joe Smith", statBufferJoe));
  statsMap->insert(pair<string, BoundedBuffer*>("John Doe", statBufferJohn));
  
  vector<RequestChannel*> workReqChans;
  
  eventHandlerArgs* eventHandlerArg = new eventHandlerArgs(reqBuffer, &workReqChans, statsMap);
  
  //threads
	pthread_t worker;
	
	pthread_t statJane;
	pthread_t statJoe;
	pthread_t statJohn;
	
	pthread_t reqJane;
	pthread_t reqJoe;
	pthread_t reqJohn;
	
	struct timeval start, end;
	//start timer
	gettimeofday(&start, NULL);
	
	
	RequestChannel* workChannel;
	
	//work thread channels
	for (int i = 0; i < numReqChannels; i++)
	{
	  string new_thread = chan.send_request("newthread");
	  workChannel = new RequestChannel(new_thread, RequestChannel::CLIENT_SIDE);
	  workReqChans.push_back(workChannel);
	}
	
	pthread_create(&worker, NULL, eventHandlerFunc, eventHandlerArg); 

	//req threads
	pthread_create(&reqJane, NULL, reqFunc, reqsJane);
	pthread_create(&reqJoe, NULL, reqFunc, reqsJoe);
	pthread_create(&reqJohn, NULL, reqFunc, reqsJohn);
	
	//stat threads
	pthread_create(&statJane, NULL, statsFunc, statsJane);
	pthread_create(&statJoe, NULL, statsFunc, statsJoe);
	pthread_create(&statJohn, NULL, statsFunc, statsJohn);
	
	pthread_join(statJane, NULL);
	pthread_join(statJoe, NULL);
	pthread_join(statJohn, NULL);
	
	//end timer
	gettimeofday(&end, NULL);
	
	//calculate elapsed time
	int64_t startTime = (start.tv_sec*1e6)+start.tv_usec;
	int64_t endTime = (end.tv_sec*1e6)+end.tv_usec;
	double elapsedTime = (endTime - startTime)*1e-6;
	printf("\nthread exec time: %f sec\n", elapsedTime);
	
	//export hisotgrams to xls file
	if (exData == true) {
		//create file
		ofstream fileJane("JaneHistogram.xls");
		ofstream fileJoe("JoeHistogram.xls");
		ofstream fileJohn("JohnHistogram.xls"); 
		
		//write to file
		buildHistFile(fileJane, histJane);
		buildHistFile(fileJoe, histJoe);
		buildHistFile(fileJohn, histJohn);
		
		//close file from writing
		fileJane.close();
		fileJoe.close();
		fileJohn.close();
	}
	
	//print histogram in terminal
	if (printHistogram == true) {
		printHist(histJane);
		printHist(histJoe);
		printHist(histJohn);
	}

  string reply4 = chan.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;
  system("rm fifo_*"); //Remove pipes
  
  /*
	double avgReqTime = totalReqTime/nRequests;
	//cout << "avg send_request request time for " << nRequests << " requests is " << avgReqTime*1e6 << " microseconds\n";
    printf("avg send_request request time for %d requests is %f microseconds\n", nRequests, avgReqTime);
    usleep(1000000);
	*/
}