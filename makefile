all: hexaPrint

hexaPrint: 2.o
	gcc -m32 -g -Wall -o hexaPrint 2.o

2.o: 2.c
	gcc -g -Wall -m32 -c -o 2.o 2.c
	
	
.PHONY: clean

clean:
	rm -f *.o hexaPrint
