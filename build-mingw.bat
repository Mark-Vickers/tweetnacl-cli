@PATH=%PATH%;C:\MinGW\bin

del *.o
del /q bin\*.*

gcc -Wall -c src/tweetnacl.c
gcc -Wall -c src/randombytes.c
gcc -Wall -c src/common.c

gcc -Wall -c src/tweetnacl-keypair.c
gcc -o bin/tweetnacl-keypair.exe tweetnacl-keypair.o tweetnacl.o randombytes.o -mconsole

gcc -Wall -c src/tweetnacl-dumpkeys.c
gcc -o bin/tweetnacl-dumpkeys.exe tweetnacl-dumpkeys.o common.o -mconsole

gcc -Wall -c src/tweetnacl-encode.c
gcc -o bin/tweetnacl-encode tweetnacl-encode.o common.o tweetnacl.o randombytes.o

gcc -Wall -c src/tweetnacl-decode.c
gcc -o bin/tweetnacl-decode tweetnacl-decode.o common.o tweetnacl.o randombytes.o

del *.o

pause
