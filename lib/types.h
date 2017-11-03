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
#define MAX_FILTERS 20


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

typedef struct phound_options {
  int put_wlan_in_monitor;
  int mode;
  int timeout;
  char * filters[MAX_FILTERS];
} PhoundOptions;

/*Function Prototypes*/
Device * make_device(char *, bpf_u_int32, bpf_u_int32);
PhoundOptions * set_default_opts();
PhoundOptions * set_opts(int, int, int, char *[]);

Device * make_device(char * name, bpf_u_int32 mask, bpf_u_int32 net){
	Device * d = (Device *) malloc(sizeof(Device));
	d->device_name = name;
	d->mask = mask;
	d->net = net;
	return d;
}
/* Some default options */
PhoundOptions * set_default_opts(){
  char * filters[MAX_FILTERS];
  return set_opts(1, PROMISC, TIMEOUT, filters);
}
/* Some custom options */
PhoundOptions * set_opts(int put_wlan_in_monitor, int mode, int timeout, char *filters[]){
  PhoundOptions * opts = (PhoundOptions *) malloc(sizeof(PhoundOptions));
  opts->mode = mode;
  opts->put_wlan_in_monitor = put_wlan_in_monitor;
  opts->timeout = timeout;
  opts->filters = filters;
  return opts;
}

#endif
