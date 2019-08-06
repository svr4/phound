#include "headers/phound_options.h"

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