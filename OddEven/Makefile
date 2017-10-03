CC = mpicc 
FLAG = -O3 

clean:
	rm *.o oddeven 

all: main.o quicksort.o
	$(CC) $(FLAG) main.o quicksort.o -o oddeven 

main.o: main.c
	$(CC) $(FLAG) -c main.c 

quicksort.o: quicksort.c
	$(CC) $(FLAG) -c quicksort.c
