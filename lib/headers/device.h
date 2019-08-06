#ifndef DEVICE_H
#define DEVICE_H

#ifndef TYPES
#include "types.h"
#endif

Device * make_device(char *, bpf_u_int32, bpf_u_int32);

#endif