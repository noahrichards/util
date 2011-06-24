#
# Created by makemake (Darwin Jun 23 2011) on Thu Jun 23 21:15:50 2011
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

########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	-ggdb -Wall -ansi -pedantic
CFLAGS =	-ggdb -Wall -ansi -pedantic
BINDIR =./bin
CLIBFLAGS =	-lm
CCLIBFLAGS =	
########## End of default flags


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

all:	${BINDIR}/makemake 

${BINDIR}/makemake:	makemake.o $(OBJFILES)
	@mkdir -p ${BINDIR}
	$(CXX) $(CXXFLAGS) -o ${BINDIR}/makemake makemake.o $(OBJFILES) $(CCLIBFLAGS)

########## Custom targets from custom_targets.mak

install:	${BINDIR}/makemake
	cp ${BINDIR}/makemake ~/bin/makemake

########## End targets from custom_targets.mak

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
