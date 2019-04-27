#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"
#include "common.h"

#define filename_MAXLENGTH 256
#define filename_SAFELENGTH (filename_MAXLENGTH - 20)
extern void randombytes(u8 *, u64);

int main(int argc, char *argv[])
{
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	unsigned char sk[crypto_box_SECRETKEYBYTES];
	FILE *fp;

	char plaintext_filename[filename_MAXLENGTH];
	char crypttext_filename[filename_MAXLENGTH];

	char public_filename[filename_MAXLENGTH];
	char secret_filename[filename_MAXLENGTH];

	unsigned char nonce[crypto_box_NONCEBYTES];
	unsigned char *plaintext;
 	unsigned long long plaintext_len;
 	unsigned char *crypttext;
 	unsigned long long crypttext_len;

	strcpy(plaintext_filename, "plaintext.txt");
	strcpy(crypttext_filename, "crypttext.txt");
	
	strcpy(public_filename, "key.public");
	strcpy(secret_filename, "key.secret");

	// usage: tweetnacl_encode filename secretkey publickey
	if (argc > 1)
	{
		strncpy(plaintext_filename, argv[1], filename_SAFELENGTH);
		strcat(plaintext_filename, ".txt");

		strncpy(crypttext_filename, argv[1], filename_SAFELENGTH);
		strcat(crypttext_filename, ".tnacl");

		strncpy(secret_filename, argv[2], filename_SAFELENGTH);
		strcat(secret_filename, ".secret");

		strncpy(public_filename, argv[3], filename_SAFELENGTH);
		strcat(public_filename, ".public");
	}

	// load in the plaintext
	fp = fopen(plaintext_filename, "rb");
	if (fp == NULL)
	{
		printf("ERROR: %s file not found!\n", plaintext_filename);
		return (EXIT_FAILURE);
	}
	fseek(fp, 0, SEEK_END);		// seek to end of file
	plaintext_len = ftell(fp);	// get current file pointer
	fseek(fp, 0, SEEK_SET);		// seek back to beginning of file
	plaintext = calloc(plaintext_len+crypto_box_ZEROBYTES, sizeof(char));
	if (plaintext) {
		fread(plaintext+crypto_box_ZEROBYTES, sizeof(char), plaintext_len, fp);
	} else {
		#ifdef WIN32
		printf("ERROR: failed to allocate %I64u bytes for plaintext!\n", plaintext_len);
		#else
		printf("ERROR: failed to allocate %lld bytes for plaintext!\n", plaintext_len);
		#endif
		return (EXIT_FAILURE);
	}
	fclose(fp);

	// load in public key of receiver
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

	// load in secret key of sender
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

	// generate a nonce
	randombytes(nonce, crypto_box_NONCEBYTES);

	// allocate space for the encrypted message
	crypttext_len = plaintext_len + crypto_box_ZEROBYTES;
	crypttext = calloc(crypttext_len, sizeof(char));
	if (!crypttext) {
		#ifdef WIN32
		printf("ERROR: failed to allocate %I64u bytes for crypttext!\n", crypttext_len);
		#else
		printf("ERROR: failed to allocate %lld bytes for crypttext!\n", crypttext_len);
		#endif
		return (EXIT_FAILURE);
	}

	// encrypt the message
	crypto_box(crypttext, plaintext, crypttext_len, nonce, pk, sk);

	// store the nonce+crypttext in a file
	fp = fopen(crypttext_filename, "wb");
	if (fp == NULL)
	{
		printf("ERROR: %s failed to open!\n", crypttext_filename);
		return (EXIT_FAILURE);
	}

	fwrite(nonce, sizeof(char), crypto_box_NONCEBYTES, fp);
	fwrite(crypttext, sizeof(char), crypttext_len, fp);

	fclose(fp);

	return (EXIT_SUCCESS);
}
