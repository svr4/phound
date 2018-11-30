#ifndef TYPES
#define TYPES

#ifndef PHOUND
#include <pcap.h>
#endif
#include <stdio.h>
#include <stdlib.h>

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
  char * filters;
} PhoundOptions;

/*Function Prototypes*/
Device * make_device(char *, bpf_u_int32, bpf_u_int32);
PhoundOptions * set_default_opts();
PhoundOptions * set_opts(int, int, int, char *);

#endif
