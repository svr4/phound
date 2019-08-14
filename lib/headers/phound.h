#ifndef PHOUND
#define PHOUND

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "phound_types.h"
#include "thread_list.h"
#include "device_list.h"

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

/* Global values */
/* List of devices */
static pcap_if_t ** pcap_device_list;
// PCAP error message.
char errbuff[PCAP_ERRBUF_SIZE];
//static pthread_t ** reading_threads;
static PacketImplementation ** implementations;
// Thread random number.
static int thread_rand_int;
// Thread name size.
static size_t t_name_size = 16;
// Thread name.
static char * t_name;

/* Function Prototypes */
// Will load devices into a list that can be queried and used.
// Will init the Packet Implementations.
int phound_init(PhoundOptions *);
// Reloads the device list of the system
int phound_reload_devices();
// Returns all connected devices
pcap_if_t ** phound_connected_devices();
// Sends a function into a new proc to read from device
void phound_read_from_device();
// Stops reading on a device
void phound_stop_read_on_device();
// Will handle the packet when they come in. It will be sent to the code
// that the developer sent in the Packet Implementation.
static void phound_handle_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
// Clean up.
int phound_stop();


#endif