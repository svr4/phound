#include "lib/headers/phound.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * handle_packet(const struct pcap_pkthdr *, const u_char *);
void packet_implementation(void *, ...);

int main(int argc, char *argv[]){

	PhoundOptions * opts = (PhoundOptions *) malloc(sizeof(PhoundOptions));
	PacketImplementation * pki = (PacketImplementation *) malloc(sizeof(PacketImplementation));
	Device * d = (Device *) malloc(sizeof(Device));

	d->device_name = "en0";
	pki->devices = &d;
	pki->filters = "src net 192.168.1.70";
	pki->pkti_handle_pkt = handle_packet;
	pki->pkti_implementation = packet_implementation;
	
	opts->impl = &pki;

	phound_init(opts);

	printf("Hello, Phound!\n");

	phound_read_from_device();

	sleep(10);

	phound_stop_read_on_device();

	phound_stop();

	return 0;
}

void * handle_packet(const struct pcap_pkthdr *header, const u_char *packet)
{
	printf("Handle packet!");
	return (void *)packet;
}

void packet_implementation(void * packet, ...){
	printf("packet implementation!");
}