#ifndef PHOUND_TYPES
#define PHOUND_TYPES

#include <pcap.h>
#include <sys/types.h>

#define PROMISC 1
#define NOTPROMISC 0
#define TIMEOUT 1

typedef struct device
{
    char *device_name;
    bpf_u_int32 mask;
    bpf_u_int32 net;
    pcap_t *handle;

} Device;

typedef struct packet_impl
{
    /* Function pointer for developer or application supplied packet handling function. */
    /* Will be used to construct the packet that they will handle and send it to the packet implementation func ptr. */
    void * (*pkti_handle_pkt)(const struct pcap_pkthdr *header, const u_char *packet);

    /* Function that a developer or application implements that will execute some code on the supplied packet of the type they filtered. */
    void (*pkti_implementation)(void *, ...);

    /* List of devices to apply the filter to read on. */
    Device ** devices;

    /* Count of devices */
    int dev_ctr;

    /* Filters that will be applied for the packet implementation. */
    char * filters;

} PacketImplementation;

typedef struct packet_impl_light PacketImplementationLight;

typedef struct phound_opts
{
    /* List of implementations */
    PacketImplementation ** impl;
    /* Count of implementations */
    int implt_ctr;

} PhoundOptions;

typedef struct t_node {
        struct t_node * next;
        struct t_node * previous;
        pthread_t * thread;
} ThreadNode;

typedef struct d_node {
        struct d_node * next;
        struct d_node * previous;
        Device * device;
} DeviceNode;

#endif