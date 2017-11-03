phound: lib/linked_list.h lib/types.h lib/phound.h main.c
	gcc lib/phound.h
	gcc main.c -o phound.o -lpcap -pthread
