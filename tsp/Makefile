all:	main.o	read.o
	gcc -O3 main.o -std=c11 -lpthread -o tsp

clean:
	rm *.o tsp

main.o: main.c 
	gcc -O3 -c -std=c11 -lpthread  main.c

read.o: read.c
	gcc -O3 -c -std=c11 -lpthread  read.c
