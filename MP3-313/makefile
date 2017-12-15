# makefile

all: client dataserver

dataserver: dataserver.C reqchannel.o
	g++ -g -o dataserver dataserver.C reqchannel.o -lpthread

client: client.C reqchannel.o bounded_buffer.o mutex.o semaphore.o
	g++ -g -o client client.C reqchannel.o bounded_buffer.o mutex.o semaphore.o -lpthread

reqchannel.o: reqchannel.C reqchannel.H
	g++ -c -g reqchannel.C

mutex.o: mutex.H mutex.C
	g++ -c -g mutex.C
	
bounded_buffer.o: bounded_buffer.H bounded_buffer.C
	g++ -c -g bounded_buffer.C

semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C
	
clean:
	rm *.o client dataserver