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

#include "types.h"
#include "linked_list.h"
#include "device_list.h"
#include "device.h"
#include "phound_options.h"


#define ETH_HEADER_SIZE sizeof(struct ether_header)
#define IP_HEADER_SIZE sizeof(struct ip)
#define TCP_HEADER_SIZE sizeof(struct tcphdr)

static pcap_t *handle;
static char errbuf[PCAP_ERRBUF_SIZE];
static struct pcap_pkthdr header;
static u_char *packet;
//static ScreenPacket * screen_buffer[10];
//static int screen_count = 0;

/*Function Prototypes*/
void got_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
int init(PhoundOptions *);
pthread_t readFromDevice(Node *);
Node * find_device(char *);
static void *readOnThread(void *);
/******/


#endif
