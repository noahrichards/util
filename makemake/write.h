/* write.h
 *
 * $Id: write.h,v 1.1 2006/06/08 02:13:59 swm Exp $
 *
 * $Log: write.h,v $
 * Revision 1.1  2006/06/08 02:13:59  swm
 * Initial revision
 *
 */

// File: write.h
// Author: K. Reek
// Contributors: S. Marshall
// Description: A class that writes the makefile.

#ifndef _WRITE_H
#define _WRITE_H

#include <string>
#include <iostream>

#include "dirlist.h"

using namespace std;

/* Class that writes the makefile
 */
class Write {
public: // Constructor

  Write( DirList &dirlist, bool noDebugFlag, bool useGCC, const string &header_file, const string &programs_file );

private: // Helper functions

  void write_header(bool noDebugFlag, bool useGCC, string header_file);
  void write_lists( DirList &dirlist );
  void write_main_targets( DirList &dirlist, string programs_file );
  void write_main_target_list( const set<string> &list,
                               string compile,
                               string local_libs );
  void write_dependencies( DirList &dirlist );
  void write_dependency_list( const set<string> &list, DirList &dirlist );
  void write_trailer( DirList &dirlist, bool useGCC  );

  set<string> products;

}; // Write

#endif
