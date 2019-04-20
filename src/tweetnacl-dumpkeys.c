#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"
#include "common.h"

#define public_filename_MAXLENGTH 256
#define secret_filename_MAXLENGTH 256
#define public_filename_SAFELENGTH (public_filename_MAXLENGTH - 20)
#define secret_filename_SAFELENGTH (secret_filename_MAXLENGTH - 20)

int main(int argc, char *argv[])
{
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	unsigned char sk[crypto_box_SECRETKEYBYTES];
	FILE *fp;

	char public_filename[public_filename_MAXLENGTH];
	char secret_filename[secret_filename_MAXLENGTH];

	strcpy(public_filename, "key.public");
	strcpy(secret_filename, "key.secret");

	if (argc > 1)
	{
		strncpy(public_filename, argv[1], public_filename_SAFELENGTH);
		strcat(public_filename, ".public");

		strncpy(secret_filename, argv[1], secret_filename_SAFELENGTH);
		strcat(secret_filename, ".secret");
	}

	fp = fopen(public_filename, "rb");
	if (fp == NULL)
	{
		printf("ERROR: %s file not found!\n", public_filename);
		return (EXIT_FAILURE);
	}
	if (fread(pk, 1, crypto_box_PUBLICKEYBYTES, fp) != crypto_box_PUBLICKEYBYTES)
	{
		printf("ERROR: %s file incorrect length!\n", public_filename);
		return (EXIT_FAILURE);
	}
	fclose(fp);

	printf("public key...\n");
	dump_key(pk, crypto_box_PUBLICKEYBYTES);

	fp = fopen(secret_filename, "rb");
	if (fp == NULL)
	{
		printf("ERROR: %s file not found!\n", secret_filename);
		return (EXIT_FAILURE);
	}
	if (fread(sk, 1, crypto_box_SECRETKEYBYTES, fp) != crypto_box_SECRETKEYBYTES)
	{
		printf("ERROR: %s file incorrect length!\n", secret_filename);
		return (EXIT_FAILURE);
	}
	fclose(fp);

	printf("secret key...\n");
	dump_key(sk, crypto_box_SECRETKEYBYTES);

	return (EXIT_SUCCESS);
}
