#include <stdio.h>

void dump_args(int argc, char *argv[])
{
	int ii;

	printf("[%d]", argc);
	for (ii = 0; ii < argc; ii++)
	{
		printf(" [%s]", argv[ii]);
	}
	printf("\n");
}

void dump_key(unsigned char key[], int length)
{
	int ii;

	for (ii = 0; ii < length; ii++)
	{
		printf("%02x", key[ii]);
	}
	printf("\n");
}
