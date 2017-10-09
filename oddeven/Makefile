CC = mpicc 
FLAG = -O3 

all: main.o quicksort.o
	$(CC) $(FLAG) main.o quicksort.o -o oddeven 

rmtxt:
	rm *.txt 

clean:
	rm *.o oddeven  

main.o: main.c
	$(CC) $(FLAG) -c main.c 

quicksort.o: quicksort.c
	$(CC) $(FLAG) -c quicksort.c
