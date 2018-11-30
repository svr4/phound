#ifndef LINKED_LIST
#define LINKED_LIST

#ifndef TYPES
#include "types.h"
#endif

static Node *head;

/*Function Prototypes*/
Node * init_node(Node *);
Device * get_device(Node *);
int add_node(Node *);
int size();
Node * find_device(char *);
/*End function prototypes*/


#endif
