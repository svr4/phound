#ifndef PHOUND
#include "phound.h"
#endif

#ifndef TYPES
#define TYPES


typedef struct device{

        char *device_name;
        bpf_u_int32 mask;
        bpf_u_int32 net;
}Device;

typedef struct node {
        struct node * next;
        struct node * previous;
        Device *device;
        pcap_t *handle;
} Node;


Device make_device(char *, bpf_u_int32, bpf_u_int32);


Device make_device(char * name, bpf_u_int32 mask, bpf_u_int32 net){

	Device d;
	d.device_name = name;
	d.mask = mask;
	d.net = net;
	return d;
}

#endif
