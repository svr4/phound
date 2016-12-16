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

//#ifndef TYPES
#include "types.h"
//#endif

//#ifndef LINKED_LIST
#include "linked_list.h"
//#endif


#define ETH_HEADER_SIZE sizeof(struct ether_header)
#define IP_HEADER_SIZE sizeof(struct ip)
#define TCP_HEADER_SIZE sizeof(struct tcphdr)

static pcap_t *handle;
static char errbuf[PCAP_ERRBUF_SIZE];
static struct pcap_pkthdr header;
static u_char *packet;


//static void got_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
int init(char *[], int, int);


void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	struct ether_header *ethernet_pointer;
	struct ip *ip_pointer;
	struct in_addr source, destination;
	struct tcphdr *tcp_pointer;
	u_char *payload;
	
	//printf("Size of device list: %d\n",size()); /*print the size of the list*/
	ethernet_pointer = (struct ether_header *) packet;
	ip_pointer = (struct ip *) (packet + ETH_HEADER_SIZE);
	tcp_pointer = (struct tcphdr *) (packet + ETH_HEADER_SIZE + IP_HEADER_SIZE);
	payload = (u_char *) (packet + ETH_HEADER_SIZE + IP_HEADER_SIZE + TCP_HEADER_SIZE);
	destination = ip_pointer->ip_dst;
	source = ip_pointer->ip_src;

	printf("Destination: %s:%u \n", inet_ntoa(destination), ntohs(tcp_pointer->th_dport));
	printf("Source: %s:%u \n", inet_ntoa(source), ntohs(tcp_pointer->th_sport));
	printf("Payload: %s\n\n", payload);
	
}

int init(char * devices[], int mode, int timeout)
{
	
	if(devices == NULL){
		printf("No device name found\n");
		return -1;
	}

	if(mode < 0){
		printf("No mode established\n");
		return -1;
	}

	if(timeout < 0){
		printf("No timeout established\n");
		return -1;
	}

	int i = 0;
	int length = sizeof *devices / sizeof(char *);
	/*printf("length: %d\n", length);*/
	char *device_name;
	bpf_u_int32 mask = 0;
	bpf_u_int32 net = 0;
	Device dev;
	
	for(i; i < length; i++){
		device_name = devices[i];
		if(device_name == NULL){
			fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
			return -1;
		}
		else{

			if(pcap_lookupnet(device_name, &net, &mask, errbuf) == -1){
				fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
				return -1;

			}
			else{
				dev = make_device(device_name, mask, net);
				Node *temp;
				temp = init_node(temp);
				temp->handle = pcap_open_live(dev.device_name, BUFSIZ, mode, timeout, errbuf);
				temp->device = &dev;
	
				temp->handle = pcap_open_live(temp->device->device_name, BUFSIZ, mode, timeout, errbuf);

				if(temp->handle == NULL){
					fprintf(stderr, "Couldn't open device %s\n", errbuf);
					return -1;
				}
				else{
					
					pcap_loop(temp->handle, -1, got_packet, NULL);
					add_node(temp);
					return 0;
				}
			}
		}
	}


}
#endif
