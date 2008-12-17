#
# Created by gmakemake (Sparc Dec 11 2008) on Tue Dec 16 22:39:38 2008
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

########## Flags from header.mak

CXXFLAGS =	-ggdb -Wall
CFLAGS =	-ggdb -Wall
CLIBFLAGS =	-lm
CCLIBFLAGS =	

########## End of flags from header.mak


CPP_FILES =	dirlist.cpp makemake.cpp write.cpp
C_FILES =	
S_FILES =	
H_FILES =	dirlist.h write.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	dirlist.o write.o 

#
# Main targets
#

all:	makemake 

install:	makemake
	cp makemake ~/bin/gmakemake

makemake:	makemake.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o makemake makemake.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

dirlist.o:	dirlist.h
makemake.o:	dirlist.h write.h
write.o:	dirlist.h write.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) makemake.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf makemake 
