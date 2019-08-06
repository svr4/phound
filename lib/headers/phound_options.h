#ifndef PHOUND_OPTIONS_H
#define PHOUND_OPTIONS_H

#ifndef TYPES
#include "types.h"
#endif

PhoundOptions * set_default_opts();
PhoundOptions * set_opts(int, int, int, char *);

#endif