all: phound.o

libphound.a: lib/phound.c lib/types.c lib/linked_list.c
	gcc -c lib/phound.c lib/types.c lib/linked_list.c
	ar rc libphound.a phound.o types.o linked_list.o
	rm *.o
phound.o: main.c libphound.a
	gcc main.c -o phound.o -L. -lphound -lpcap -pthread
