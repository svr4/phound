all: phound.o

libphound.a: lib/linked_list.c lib/types.c lib/phound.c
	gcc -c lib/linked_list.c lib/types.c lib/phound.c
	ar rc libphound.a linked_list.o types.o phound.o
	rm *.o
phound.o: main.c libphound.a
	gcc main.c -o phound.o -L. -lphound -lpcap -pthread
