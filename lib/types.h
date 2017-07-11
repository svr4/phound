#ifndef PHOUND
#include "phound.h"
#endif

#ifndef TYPES
#define TYPES
#define ETHERNET "eth0"
#define WIRELESS "wlan0"
#define PROMISC 1
#define NOTPROMISC 0
#define TIMEOUT 1


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

/*Function Prototypes*/
Device * make_device(char *, bpf_u_int32, bpf_u_int32);


Device * make_device(char * name, bpf_u_int32 mask, bpf_u_int32 net){
	Device * d = malloc(sizeof(Device));
	d->device_name = name;
	d->mask = mask;
	d->net = net;
	return d;
}

#endif
