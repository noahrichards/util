/* makemake.cpp
 *
 * $Id: makemake.cpp,v 1.1 2006/06/08 02:14:38 swm Exp $
 *
 * $Log: makemake.cpp,v $
 * Revision 1.1  2006/06/08 02:14:38  swm
 * Initial revision
 *
 */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "dirlist.h"
#include "write.h"

bool debug(false);

int main(int argc, char **argv) {
    string header_file("header.mak");
    string programs_file("programs.mak");
    string custom_targets_file("custom_targets.mak");

    // Flag indicates whether the CCFLAGS and CFLAGS output should
    // have -g debugging option omitted
    bool noDebugFlag(false);

    // Flag indicates using gnu compiler. Default is clang.
    bool useGCC = false;

    // check gnu compiler make or sun compiler make
    // if first letter of executable is 'g' then gnu otherwise sun

    char *exename = argv[0];
    char c;

    while ((c = *exename++)) {
        if (c == '/') {
            argv[0] = exename;
        }
    }
    if (argv[0][0] == 'g') {
        useGCC = true;
    }

    //
    // Process command line options
    //
    while (argc--, *++argv != NULL && **argv == '-') {
        if (strcmp(*argv, "-header") == 0 && argv[1] != NULL) {
            header_file = *++argv;
            argc--;
        } else if (strcmp(*argv, "-no-header") == 0) {
            header_file = "//";
        } else if (strcmp(*argv, "-programs") == 0 && argv[1] != NULL) {
            programs_file = *++argv;
            argc--;
        } else if (strcmp(*argv, "-no-programs") == 0) {
            programs_file = "//";
        } else if (strcmp(*argv, "-custom_targets") == 0 && argv[1] != NULL) {
            custom_targets_file = *++argv;
            argc--;
        } else if (strcmp(*argv, "-no-custom_targets") == 0) {
            custom_targets_file = "//";
        } else if (strcmp("-no-debug", *argv) == 0) {
            noDebugFlag = true;
        } else {
            switch (*++*argv) {
                case 'd':
                    debug = true;
                    break;

                default:
                    cerr << "Invalid option: -" << **argv << endl;
                    exit(1);
            }
        }
    }

    //
    // Read the current directory (and cmd line arguments) to find
    // file names.
    //
    DirList dirlist(argc, argv);

    //
    // Now write the makefile
    //
    Write write(dirlist, noDebugFlag, useGCC, header_file, programs_file,
                custom_targets_file);

    return 0;
}
