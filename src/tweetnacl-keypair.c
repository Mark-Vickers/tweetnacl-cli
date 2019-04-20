#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"

#define TRUE 1
#define FALSE 0
#define public_filename_MAXLENGTH 256
#define secret_filename_MAXLENGTH 256
#define public_filename_SAFELENGTH (public_filename_MAXLENGTH - 20)
#define secret_filename_SAFELENGTH (secret_filename_MAXLENGTH - 20)

void dump_args(int argc, char *argv[]);
int key_files_exist();

int main(int argc, char *argv[])
{
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

	if (!key_files_exist(public_filename, secret_filename))
	{
		unsigned char pk[crypto_box_PUBLICKEYBYTES];
		unsigned char sk[crypto_box_SECRETKEYBYTES];
		FILE *fp;

		crypto_box_keypair(pk, sk);

		fp = fopen(public_filename, "wb");
		if (fwrite(pk, 1, crypto_box_PUBLICKEYBYTES, fp) != crypto_box_PUBLICKEYBYTES)
		{
			printf("ERROR: public.key wrote incorrect file length!\n");
			return (EXIT_FAILURE);
		}
		fclose(fp);

		fp = fopen(secret_filename, "wb");
		if (fwrite(sk, 1, crypto_box_SECRETKEYBYTES, fp) != crypto_box_SECRETKEYBYTES)
		{
			printf("ERROR: secret.key wrote incorrect file length!\n");
			return (EXIT_FAILURE);
		}
		fclose(fp);

		printf("created keyfiles\n");
	}
	else
	{
		printf("ERROR: key files already exist\n");
	}

	return (EXIT_SUCCESS);
}

int key_files_exist(char *public_filename, char *secret_filename)
{
	FILE *fp;

	fp = fopen(public_filename, "r");
	if (fp == NULL)
		return (FALSE);
	fclose(fp);

	fp = fopen(secret_filename, "r");
	if (fp == NULL)
		return (FALSE);
	fclose(fp);

	return (TRUE);
}
