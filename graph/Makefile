CC=gcc 
CMP=-O3 -lpthread -o

serial: serial.c
	$(CC) serial.c $(CMP) serial.out 

static: static.c
	$(CC) static.c $(CMP) static.out 

dynamic: dynamic.c
	$(CC) dynamic.c $(CMP) dynamic.out

clean:
	rm *.out
