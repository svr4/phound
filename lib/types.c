#include "headers/types.h"

Device * make_device(char * name, bpf_u_int32 mask, bpf_u_int32 net){
	Device * d = (Device *) malloc(sizeof(Device));
	d->device_name = name;
	d->mask = mask;
	d->net = net;
	return d;
}
/* Some default options */
PhoundOptions * set_default_opts(){
  char * filters = (char *)malloc(sizeof(char) * MAX_FILTERS);
  return set_opts(1, PROMISC, TIMEOUT, filters);
}
/* Some custom options */
PhoundOptions * set_opts(int put_wlan_in_monitor, int mode, int timeout, char *filters){
  PhoundOptions * opts = (PhoundOptions *) malloc(sizeof(PhoundOptions));
  opts->mode = mode;
  opts->put_wlan_in_monitor = put_wlan_in_monitor;
  opts->timeout = timeout;
  opts->filters = filters;
  return opts;
}