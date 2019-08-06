#include "headers/device.h"

Device * make_device(char * name, bpf_u_int32 mask, bpf_u_int32 net){
	Device * d = (Device *) malloc(sizeof(Device));
	d->device_name = name;
	d->mask = mask;
	d->net = net;
	return d;
}