#include<stdio.h>
#include<pcap.h>


int main(int argc, char* argv[])
{
	printf("%d\n", argc);

	int count = 0;

	for(count = 0; count < argc; count++)
	{
		printf("%s\n", argv[count]);

	}

	return 0;
}
