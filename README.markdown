A few different utility programs that I've collected.

makemake
========

Written by Sidney Marshall <swm@cs.rit.edu>.

This program automatically generates a simple Makefile for directories
of C and C++ files.  Depending on the name of the binary (`gmakemake` or
`makemake`), the program will create a slightly different makefile.
`gmakemake` generates one suitable for GCC/G++.

I've made a few small fixes and changes (like including
`-Wall -ansi -pedantic` in the build flags).

The original can be found at:

[Makemake homepage @ cs.rit.edu](http://www.cs.rit.edu/~swm/makemake/)
