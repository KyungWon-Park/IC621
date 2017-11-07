all:	main.o
	gcc -O3 main.o -lpthread -o pi 

clean:
	rm *.o pi

main.o: main.c 
	gcc -O3 -c -lpthread  main.c
