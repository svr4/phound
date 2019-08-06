all: phound.o

libphound.a: lib/phound.c lib/device.c lib/phound_options.c lib/device_list.c
	gcc -c lib/phound.c lib/device.c lib/phound_options.c lib/device_list.c
	ar rc libphound.a phound.o device.o phound_options.o device_list.o
	rm *.o
phound.o: main.c libphound.a
	gcc main.c -o phound.o -L. -lphound -lpcap -pthread
