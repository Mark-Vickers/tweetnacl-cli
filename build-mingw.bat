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

del *.o

pause
