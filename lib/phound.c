#include "headers/phound.h"

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	struct ether_header *ethernet_pointer = (struct ether_header *) malloc(sizeof(struct ether_header));
	struct ip *ip_pointer = (struct ip *) malloc(sizeof(struct ip));
	struct in_addr source, destination;
	struct tcphdr *tcp_pointer = (struct tcphdr *) malloc(sizeof(struct tcphdr));
	u_char *payload = (u_char *) malloc(sizeof(u_char));

	//printf("Size of device list: %d\n",size()); /*print the size of the list*/
	ethernet_pointer = (struct ether_header *) packet;
	ip_pointer = (struct ip *) (packet + ETH_HEADER_SIZE);
	tcp_pointer = (struct tcphdr *) (packet + ETH_HEADER_SIZE + IP_HEADER_SIZE);
	payload = (u_char *) (packet + ETH_HEADER_SIZE + IP_HEADER_SIZE + TCP_HEADER_SIZE);
	destination = ip_pointer->ip_dst;
	source = ip_pointer->ip_src;

	/*printf("Destination: %s:%u \n", inet_ntoa(destination), ntohs(tcp_pointer->th_dport));
	printf("Source: %s:%u \n", inet_ntoa(source), ntohs(tcp_pointer->th_sport));
	printf("Payload: %s\n\n", payload);*/

	struct tm * timeinfo = (struct tm *) malloc(sizeof(struct tm));
	char *timestamp = (char *) malloc(sizeof(char) * 26);
	ctime_r(&header->ts.tv_sec, timestamp);

	printf("Sequence: %u Ack: %u Window: %u IP id: %u Timestamp: %s\n", tcp_pointer->th_seq, tcp_pointer->th_ack, tcp_pointer->th_win,
	ip_pointer->ip_id, timestamp);
	// if (tcp_pointer->th_flags & TH_FIN){
    //     printf("\tFlag: TH_FIN\n");
    // }
    // if (tcp_pointer->th_flags & TH_SYN){
    //     printf("\tFlag: TH_SYN\n");
	// }
	// if (tcp_pointer->th_flags & TH_RST){
    //     printf("\tFlag: TH_RST\n");
	// }
	// if (tcp_pointer->th_flags & TH_PUSH){
    //     printf("\tFlag: TH_PUSH\n");
	// }
	// if (tcp_pointer->th_flags & TH_ACK){
    //     printf("\tFlag: TH_ACK\n");
	// }
	// if (tcp_pointer->th_flags & TH_URG){
    //     printf("\tFlag: TH_URG\n");
	// }
	// if (tcp_pointer->th_flags & TH_ECE){
    //     printf("\tFlag: TH_ECE\n");
	// }
	// if (tcp_pointer->th_flags & TH_CWR){
    //     printf("\tFlag: TH_CWR\n");
	// }
	//printf("%s:%u %s:%u %s\n", inet_ntoa(destination), ntohs(tcp_pointer->th_dport), inet_ntoa(source), ntohs(tcp_pointer->th_sport), payload);

	/*ScreenPacket * pkt = malloc(sizeof(ScreenPacket));
	pkt->destination = inet_ntoa(destination);
	pkt->d_port = ntohs(tcp_pointer->th_dport);
	pkt->source = inet_ntoa(source);
	pkt->s_port = ntohs(tcp_pointer->th_sport);
	pkt->payload = strdup(payload);*/

	/*if(screen_count < 10){
		screen_buffer[screen_count] = pkt;
		screen_count++;
	}
	else{
		screen_count = 0;
	}*/

}

pthread_t readFromDevice(Node * node)
{
	pthread_t thread;
	if(pthread_create(&thread, NULL, readOnThread, (void *)node) != 0)
		printf("Error creating reading thread...\n");
	return thread;
}

Node * find_device(char * name)
{
	return dlist_find_device(name);
}

/*Reading Thread*/
static void *readOnThread(void * n)
{
	Node *node = (Node*) n;

	while(1){
		if(pcap_dispatch(node->handle, 1, got_packet, NULL) < 0){
			printf("Error on reading from node..\n");
		}
	}

	//int i = 0;
	/* TODO: Receive struct with packet details to print on screen */
	/*for(i; i < screen_count; i++){
		if(screen_buffer[i] != NULL){
			ScreenPacket *pkt = (ScreenPacket*)screen_buffer[i];
			printf("%s:%u %s:%u %s\n", pkt->destination, pkt->d_port, pkt->source, pkt->s_port, pkt->payload);
			pkt = NULL;
		}
	}*/
}
/* Initialize the sniffer on Ethernet */
int init(PhoundOptions * opts)
{
	//*screen_buffer = (char*) malloc(10 * sizeof(int));
	//*screen_buffer = (ScreenPacket*) malloc(10 * sizeof(int));

	/* Get the options */
	int put_wlan_in_monitor = opts->put_wlan_in_monitor;
	int mode = opts->mode;
	int timeout = opts->timeout;
	char * filters = opts->filters;

	if(filters == NULL){
		printf("Filters are null\n");
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
	int length = sizeof *filters / sizeof(char *);
	int hasFilters = 0;
	/* No filters established, set to 1*/
	if(length > 0){
		hasFilters = 1;
	}

	/* Automatically looks up for your device */
	char *device_name = pcap_lookupdev(errbuf);
	bpf_u_int32 mask = 0;
	bpf_u_int32 net = 0;
	Device * dev;

	struct bpf_program fp; /*The compiled filter expression*/

	if(device_name == NULL){
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return -1;
	}
	else{

		if(pcap_lookupnet(device_name, &net, &mask, errbuf) == -1){
			fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device_name, errbuf);
			return -1;
		}
		else{
			printf("Device: %s\n", device_name);
			dev = make_device(device_name, mask, net);
			Node *temp;
			temp = (Node*)init_node(temp);
			temp->handle = pcap_open_live(dev->device_name, BUFSIZ, mode, timeout, errbuf);
			temp->device = dev;
			pcap_set_tstamp_type(temp->handle, PCAP_TSTAMP_ADAPTER);

			if(temp->handle == NULL){
				fprintf(stderr, "Couldn't open device %s\n", errbuf);
				return -1;
			}
			else{
				/* Try to set to monitor if device is wlan */
				// TODO: Actually set handle to monitor mode;

				if(put_wlan_in_monitor && (strstr(device_name, "wlan") != NULL)){
					int can_set_to_monitor = pcap_can_set_rfmon(temp->handle);
					if(can_set_to_monitor == 1){
						printf("Can set to monitor\n");
					}
					else if(can_set_to_monitor == 0){
						printf("Cannot set to monitor\n");
					}
					else{
						fprintf(stderr, "Couldn't set to monitor mode: %s\n", pcap_geterr(temp->handle));
						return -1;
					}
				}

				if(hasFilters){
					/* For each filter */
					for(i; i < length; i++){
						if(pcap_compile(temp->handle, &fp, (filters + i), 0, net) == -1){
							fprintf(stderr, "Couldn't parse filter %s: %s\n", (filters + i), pcap_geterr(temp->handle));
							return -1;
						}
						/*set filter after compilation*/
						if(pcap_setfilter(temp->handle, &fp) == -1){
							fprintf(stderr, "Couldn't install filter %s: %s\n", (filters + i), pcap_geterr(temp->handle));
							return -1;
						}
					}
				}

				if(pcap_setnonblock(temp->handle, 1, errbuf) == -1){
					fprintf(stderr, "Couldn't set the device to non blocking: %s\n", errbuf);
					return -1;
				}
				add_node(temp);
				return 0;
			}
		}
	}

}