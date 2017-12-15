# makefile

all: memtest

free_list.o: free_list.c free_list.h
	g++ -c -g -o free_list.o free_list.c

my_allocator.o: my_allocator.c my_allocator.h
	g++ -c -g -o my_allocator.o my_allocator.c

memtest: memtest.c free_list.o my_allocator.o
	g++ -g -o memtest memtest.c free_list.o my_allocator.o

