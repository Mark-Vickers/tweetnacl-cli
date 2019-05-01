# Working Out How To Use TweetNaCl

## Introduction
I wanted to use TweetNaCl for an application. I liked the fact that it was just
a code and header file that I could include in any application and I'd have
good crypto.

TweetNaCl sacrifices speed for size. My application was not high throughput,
so the slower crypto was not a problem.

## First Steps
There are very few hits for a search on using TweetNaCl. In fact the search
overwhelmingly returns results for the javascript variant which seems very
popular.

In the end I followed the link on TweetNaCl's site to it's bigger brother NaCl
itself. Here was some example code at least. However, I immediately hit a snag.
Compiling any code using tweetnacl.c and tweetnacl.h results in an error that
there is no definition of the randombytes function.

After head scratching and further searching the few available links I discover
that the randombytes function is called to obtain a random sequence of bytes a
buffer. Obvious. Over an hour or two I developed a routine that works on both
windows and linux using the OS's own methods for doing this. If the OS specific
method fails it falls back to using built in C functions. This is not ideal as
it uses the current time as a seed to the random number generator. This could
be discovered by brute force, but it is better than nothing. So now I have a
randombytes.c file that now allows clean compilation of code using tweetnacl.

Note that the windows version of this code uses the deprecated API functions
`CryptAquireContext`, `CryptGenRandom` and `CryptReleaseContext`. The Dev Center
website recommends using the Cryptography Next Generation APIs. However, my code
may need to run on older versions of Windows that don't support this.

# First working version
After a little work and hair pulling I finally have some working code that will
allow the generation of keypairs and the encoding and decoding of files.

Usage is as follows...

`tweetnacl-keypair <name>`  
generates a public and secret key pair e.g.
`tweetnacl-keypair alice`  
will generate tw files called `alice.public` and `alice.secret`

`tweetnacl-dumpkeys <name>`  
dumps out the public and secret key pairs. This is only useful to allow the public
key to be transported as a string.

`tweetnacl-storekey <name> <textkey>`  
converts the textkey produced by `tweetnacl-dumpkeys` back into a binary file e.g.
`tweetnacl-storekey alice cb81792f4c044fcd8bc3e7c6d4595db1e9bc117ca68c316b8930d2b397309624`
converts the text string back into binary and stores it in `alice.public`

`tweetnacl-encode <file> <sender> <receiver>`  
takes the file `<file>.txt` and using the sender secret key and the receivers
public key encrypts the contents and puts it in `<file>.tnacl` e.g.
`tweetnacl-encode message alice bob`
takes `message.txt` and using `alice.secret` and `bob.public` encrypts `message.txt`
and puts the result in `message.tnacl`.

`tweetnacl-decode <file> <sender> <receiver>`  
takes the file `<file>.tnacl` and using the senders public key and the receivers
private key decrypts the contents and puts it in `<file>.txt` e.g.
`tweetnacl-decode message alice bob`
takes `message.tnacl` and using `alice.public` and `bob.secret` decrypts `message.tnacl`
and puts the result in `message.txt`


