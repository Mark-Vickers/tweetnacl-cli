#!/bin/sh

rm bin/*

gcc -Wall -c src/tweetnacl.c
gcc -Wall -c src/randombytes.c
gcc -Wall -c src/common.c

gcc -Wall -c src/tweetnacl-keypair.c
gcc -o bin/tweetnacl-keypair tweetnacl-keypair.o tweetnacl.o randombytes.o

gcc -Wall -c src/tweetnacl-dumpkeys.c
gcc -o bin/tweetnacl-dumpkeys tweetnacl-dumpkeys.o common.o

rm *.o
