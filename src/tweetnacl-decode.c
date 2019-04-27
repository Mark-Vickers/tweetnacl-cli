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

	// usage: tweetnacl_decode filename publickey secretkey
	if (argc > 1)
	{
		strncpy(crypttext_filename, argv[1], filename_SAFELENGTH);
		strcat(crypttext_filename, ".tnacl");

		strncpy(plaintext_filename, argv[1], filename_SAFELENGTH);
		strcat(plaintext_filename, ".txt");

		strncpy(public_filename, argv[2], filename_SAFELENGTH);
		strcat(public_filename, ".public");

		strncpy(secret_filename, argv[3], filename_SAFELENGTH);
		strcat(secret_filename, ".secret");
	}

	// load in the nonce and crypttext
	fp = fopen(crypttext_filename, "rb");
	if (fp == NULL)
	{
		printf("ERROR: %s file not found!\n", crypttext_filename);
		return (EXIT_FAILURE);
	}
	fseek(fp, 0, SEEK_END);		// seek to end of file
	crypttext_len = ftell(fp);	// get current file pointer
	fseek(fp, 0, SEEK_SET);		// seek back to beginning of file
	crypttext_len = crypttext_len - crypto_box_NONCEBYTES;
	crypttext = calloc(crypttext_len+crypto_box_NONCEBYTES, sizeof(char));
	if (crypttext) {
		fread(nonce, sizeof(char), crypto_box_NONCEBYTES, fp);
		fread(crypttext, sizeof(char), crypttext_len, fp);
	} else {
		#ifdef WIN32
		printf("ERROR: failed to allocate %I64u bytes for crypttext!\n", crypttext_len);
		#else
		printf("ERROR: failed to allocate %lld bytes for crypttext!\n", crypttext_len);
		#endif
		return (EXIT_FAILURE);
	}
	fclose(fp);

	// load in public key of sender
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

	// load in secret key of receiver
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

	// allocate space for the plain text message
	plaintext_len = crypttext_len;
	plaintext = calloc(plaintext_len, sizeof(char));
	if (!plaintext) {
		#ifdef WIN32
		printf("ERROR: failed to allocate %I64u bytes for plaintext!\n", plaintext_len);
		#else
		printf("ERROR: failed to allocate %lld bytes for plaintext!\n", plaintext_len);
		#endif
		return (EXIT_FAILURE);
	}

	// decrypt the message
    crypto_box_open(plaintext, crypttext, crypttext_len, nonce, pk, sk);

	// store the plaintext in a file
	fp = fopen(plaintext_filename, "wb");
	if (fp == NULL)
	{
		printf("ERROR: %s failed to open!\n", plaintext_filename);
		return (EXIT_FAILURE);
	}
	fwrite(plaintext+crypto_box_ZEROBYTES, sizeof(char), plaintext_len-crypto_box_ZEROBYTES, fp);

	fclose(fp);

	return (EXIT_SUCCESS);
}
