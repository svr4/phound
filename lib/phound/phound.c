#include "../headers/phound.h"
/* Internal structure */
struct packet_impl_light
{
    /* Function pointer for developer or application supplied packet handling function. */
    /* Will be used to construct the packet that they will handle and send it to the packet implementation func ptr. */
    void * (*pkti_handle_pkt)(const struct pcap_pkthdr *header, const u_char *packet);

    /* Function that a developer or application implements that will execute some code on the supplied packet of the type they filtered. */
    void (*pkti_implementation)(void *, ...);

    /* List of devices to apply the filter to read on. */
    Device * device;
};
/***********************************************/

/* Local Prototypes */
int phound_setup_handle(Device *, PacketImplementation *);
static void * phound_read_on_thread(void *);

/* Function Prototypes */
// Will load devices into a list that can be queried and used.
// Will init the Packet Implementations.
int phound_init(PhoundOptions * options)
{
    implementations = options->impl;

    if(implementations != NULL)
    {
        printf("Impl not null %lu!\n", LENGTH(implementations));
        int i;
        int ii;
        PacketImplementation * pki;
        Device * d;
        for(i = 0; i < LENGTH(implementations); i++)
        {
            printf("Loop impl\n");
            pki = implementations[i];
            
            // Just setup a default device
            if(pki->devices == NULL)
            {
                printf("Setup default device \n");
                d = (Device *) malloc(sizeof(Device));
                if(phound_setup_handle(d, pki) == 0)
                {
                    pki->devices = &d;
                }
            }
            else
            {
                // loop through devices
                Device ** devices = (Device *) calloc(LENGTH(pki->devices), sizeof(Device *));

                for(ii=0; ii < LENGTH(pki->devices); ii++)
                {
                    printf("Loop devices\n");
                    d = pki->devices[ii];
                    if(phound_setup_handle(d, pki) == 0)
                    {
                        printf("Device setup Tgank %s\n", d->device_name);
                        devices[ii] = d;
                    }
                }

                pki->devices = devices;
            }

        }
    }
    else
    {
        fprintf(stderr, "Fatal error: No PacketImplementation present.");
        return 1;
    }

    return 0;
}

int phound_setup_handle(Device * d, PacketImplementation * pki)
{
    struct bpf_program fp;
    d->device_name = pcap_lookupdev(errbuff);
    if(d->device_name == NULL)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", d->device_name, errbuff);
        return -1;
    }
    else
    {
        if(pcap_lookupnet(d->device_name, &d->net, &d->mask, errbuff) == -1){
            fprintf(stderr, "Couldn't get netmask for device %s: %s\n", d->device_name, errbuff);
            return -2;
        }

        // It's all good
        d->handle = pcap_open_live(d->device_name, BUFSIZ, PROMISC, TIMEOUT, errbuff);
        if(d->handle == NULL)
        {
            fprintf(stderr, "Couldn't open device %s\n", errbuff);
            return -3;
        }
        // Timestamp the packets
        pcap_set_tstamp_type(d->handle, PCAP_TSTAMP_ADAPTER);

        if(pcap_compile(d->handle, &fp, pki->filters, 0, d->net) == -1){
            fprintf(stderr, "Couldn't parse filter %s: %s\n", pki->filters, pcap_geterr(d->handle));
            return -4;
        }
        /*set filter after compilation*/
        if(pcap_setfilter(d->handle, &fp) == -1){
            fprintf(stderr, "Couldn't install filter %s: %s\n", pki->filters, pcap_geterr(d->handle));
            return -5;
        }

    }

    return 0;
}


// Reloads the device list of the system
int phound_reload_devices()
{
    return -1;
}
// Returns all connected devices
pcap_if_t ** phound_connected_devices()
{
    return NULL;
}
// Sends a function into a new proc to read from device.
// Puts that thread in a list.
void phound_read_from_device()
{
    int i;
    int ii;
    Device * d;
    PacketImplementation * pki;
    PacketImplementationLight * pkil;
    ThreadNode * tn;
    for(i = 0; i < LENGTH(implementations); i++)
    {
        pki = implementations[i];
        for(ii=0; ii < LENGTH(pki->devices); ii++)
        {
            // Prepare device and callbacks for reading loop
            pkil = (PacketImplementationLight *) malloc(sizeof(PacketImplementationLight));
            pkil->device = pki->devices[ii];
            pkil->pkti_implementation = pki->pkti_implementation;
            pkil->pkti_handle_pkt = pki->pkti_handle_pkt;

            pthread_t thread;
            if(pthread_create(&thread, NULL, phound_read_on_thread, (void *)pkil) != 0)
		        printf("Error sending the device to read: %s\n", pkil->device->device_name);
            // Name the thread
            //pthread_setname_np(thread, "");
            // Add thread to list.
            tn = thread_list_init_node();
            tn->thread = &thread;
            thread_list_add_node(tn);
        }
    }
}
// Will be sent to another process to read
static void * phound_read_on_thread(void * node)
{
    PacketImplementationLight * pkil = (PacketImplementationLight *)node;

    while(1){
		if(pcap_dispatch(pkil->device->handle, 1, phound_handle_packet, (u_char *) pkil) < 0){
			printf("Error on reading from the device: %s..\n", pkil->device->device_name);
		}
	}
}
// Will handle the packet when they come in. It will be sent to the code
// that the developer sent in the Packet Implementation.
static void phound_handle_packet(u_char * args, const struct pcap_pkthdr * header, const u_char * packet)
{
    // get the PKIL packet by casting the args to a pointer of that type
    PacketImplementationLight * pkil = (PacketImplementationLight *)args;
    pkil->pkti_handle_pkt(header, packet);
    pkil->pkti_implementation("anything");
}

void phound_stop_read_on_device()
{
    ThreadNode * tn = thread_list;
    ThreadNode * temp;
    pthread_t * t;
    while(tn != NULL)
    {
        t = tn->thread;
        pthread_join(*t, NULL);
        temp = tn;
        tn = tn->next;
        free(temp);
    }
}

int phound_stop()
{

    DeviceNode * dn = device_list;
    DeviceNode * temp;
    while(dn != NULL){
        temp = dn;
        dn = dn->next;
        free(temp);
    }

    ThreadNode * tn = thread_list;
    ThreadNode * temp2;
    while(tn != NULL){
        temp2 = tn;
        tn = tn->next;
        free(temp2);
    }

    if(pcap_device_list != NULL)
        pcap_freealldevs(*pcap_device_list);

    int i;
    int ii;
    PacketImplementation * pki;
    Device * d;
    for(i = 0; i < LENGTH(implementations); i++)
    {
        pki = implementations[i];

        for(ii=0; ii < LENGTH(pki->devices); ii++)
        {
            d = pki->devices[ii];
            if(d != NULL)
            {
                // printf("Device: %s\n", d->device_name);
                d->device_name = NULL;
                free(d->handle);
                free(d);
            }
        }

        free(pki);
    }
    
    return 0;
}

void list_code()
{
    // Just setup a default device
    // if(pki->devices == NULL)
    // {
    //     printf("Setup default device \n");
    //     d = (Device *) malloc(sizeof(Device));
    //     if(phound_setup_handle(d, pki) == 0)
    //     {
    //         DeviceNode *n = device_list_init_node();
    //         n->device = d;
    //         device_list_add_node(n);
    //     }
    // }
    // else
    // {
    //     // loop through devices
    //     for(ii=0; ii < LENGTH(pki->devices); ii++)
    //     {
    //         printf("Loop devices\n");
    //         d = pki->devices[ii];
    //         if(phound_setup_handle(d, pki) == 0)
    //         {
    //             DeviceNode *n = device_list_init_node();
    //             n->device = d;
    //             device_list_add_node(n);
    //         }
    //     }
    // }

}