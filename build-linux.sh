#!/bin/sh

mkdir -p bin
rm bin/*

gcc -Wall -c src/tweetnacl.c
gcc -Wall -c src/randombytes.c
gcc -Wall -c src/common.c

gcc -Wall -c src/tweetnacl-keypair.c
gcc -o bin/tweetnacl-keypair tweetnacl-keypair.o tweetnacl.o randombytes.o

gcc -Wall -c src/tweetnacl-dumpkeys.c
gcc -o bin/tweetnacl-dumpkeys tweetnacl-dumpkeys.o common.o

gcc -Wall -c src/tweetnacl-storekey.c
gcc -o bin/tweetnacl-storekey tweetnacl-storekey.o

gcc -Wall -c src/tweetnacl-encode.c
gcc -o bin/tweetnacl-encode tweetnacl-encode.o common.o tweetnacl.o randombytes.o

gcc -Wall -c src/tweetnacl-decode.c
gcc -o bin/tweetnacl-decode tweetnacl-decode.o common.o tweetnacl.o randombytes.o

rm *.o
