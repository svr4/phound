#include "lib/phound.h"

int main(int argc, char *argv[]){

	char * filters[] = {};
	int success = init(PROMISC, TIMEOUT, filters);
	if(success == 0){
		printf("Success!: %d\n", success);
	}
	else{
		printf("Failed!: %d\n", success);
	}

	Node * n = find_device(ETHERNET);

	if(n == NULL)
		printf("Shits null \n");
	else
		printf("Found it!\n");

	
	pthread_t t = readFromDevice(n);
	sleep(2);
	pthread_cancel(t);

	return 0;
}
