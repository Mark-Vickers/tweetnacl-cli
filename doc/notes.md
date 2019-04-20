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

