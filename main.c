#include "lib/phound.h"

int main(int argc, char *argv[]){

	char * devices[] = {"wlan0"};
	int success = init(devices, 1, 1);
	if(success == 0){
		printf("Success!: %d\n", success);
	}
	else{
		printf("Failed!: %d\n", success);
	}

	return 0;
}
