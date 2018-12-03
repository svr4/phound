#include "lib/headers/phound.h"

int main(int argc, char *argv[]){

	/* Get some default options */
	PhoundOptions * opts = set_default_opts();
	int success = init(opts);
	/*if(success == 0){
		printf("Success!: %d\n", success);
	}
	else{
		printf("Failed!: %d\n", success);
	}*/
	// Finds the initialized device that's in the list when I call init()
	// enp3s0
	Node * n = find_device("en0");

	if(n == NULL)
		printf("Shits null \n");
	else
		printf("Found it!\n");


	pthread_t t = readFromDevice(n);

	while(1);

	return 0;
}
