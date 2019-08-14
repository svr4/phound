#ifndef TYPES
#include "phound_types.h"
#endif

static DeviceNode *device_list;
DeviceNode * device_list_init_node();
int device_list_add_node(DeviceNode *);
int device_list_size();
Device * device_list_get_device(DeviceNode *);
DeviceNode * device_list_find_device(char *);