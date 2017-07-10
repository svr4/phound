#include "lib/phound.h"

int main(int argc, char *argv[]){

	char * filters[] = {};
	int success = init(1, 1, filters);
	if(success == 0){
		printf("Success!: %d\n", success);
	}
	else{
		printf("Failed!: %d\n", success);
	}

	return 0;
}
