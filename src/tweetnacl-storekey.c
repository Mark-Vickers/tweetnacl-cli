#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"

#define public_filename_MAXLENGTH 256
#define public_filename_SAFELENGTH (public_filename_MAXLENGTH - 20)

int main(int argc, char *argv[])
{
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	FILE *fp;
	char strint[3];
	strint[0] = 0;
	strint[1] = 0;
	strint[2] = 0;

	char public_filename[public_filename_MAXLENGTH];
	char public_keystring[crypto_box_PUBLICKEYBYTES*2];

	strcpy(public_filename, "key.public");

	if (argc > 1)
	{
		strncpy(public_filename, argv[1], public_filename_SAFELENGTH);
		strcat(public_filename, ".public");
	}
	if (argc > 2) {
		strncpy(public_keystring, argv[2], (crypto_box_PUBLICKEYBYTES*2));
	}

	int ii;
	int jj;

	jj = 0;
	for (ii=0; ii<(crypto_box_PUBLICKEYBYTES*2); ii=ii+2) {
		strncpy(strint, &public_keystring[ii], 2);
		pk[jj] = (int)strtoul(strint, NULL, 16);
		jj++;
	}

	fp = fopen(public_filename, "wb");
	if (fwrite(pk, 1, crypto_box_PUBLICKEYBYTES, fp) != crypto_box_PUBLICKEYBYTES)
	{
		printf("ERROR: public.key wrote incorrect file length!\n");
		return (EXIT_FAILURE);
	}
	fclose(fp);

	return (EXIT_SUCCESS);
}
