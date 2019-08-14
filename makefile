all: phound.o

libphound.a: lib/phound/phound.c lib/phound/device_list.c lib/phound/thread_list.c
	gcc -c lib/phound/phound.c lib/phound/device_list.c lib/phound/thread_list.c
	ar rc libphound.a phound.o device_list.o thread_list.o
	rm *.o
phound.o: main.c libphound.a
	gcc main.c -o phound.o -L. -lphound -lpcap -pthread
