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

/**
 * Enum to describe the Operating System the Makefile will be run on
 */
enum OS{
	WINDOWS,//!< WINDOWS cmd.exe
	LINUX,  //!< LINUX   bash shell
	NO_OS   //!< NO_OS   not-used
};

/**
 * Enum to Describe the Compiler being used
 */
enum COMPILER{
	GCC,       //!< GCC
	CLANG,     //!< CLANG
	SUN,       //!< SUN
	NO_COMPILER//!< NO_COMPILER
};

/**
 * Class that writes the makefile to stdout
 */
class Write
{
public:

	/**
	 * Constructor for the Makefile Writer
	 * @param dirlist 				The directory listing
	 * @param noDebugFlag 			True for no debug
	 * @param comp					The Compiler to Use
	 * @param sys					The Operaing System
	 * @param header_file			File containing custom header
	 * @param programs_file			File Containing cust programs
	 * @param custom_targets_file	File Containg custom targets
	 */
  Write(DirList &dirlist, bool noDebugFlag, const COMPILER comp,const OS sys,
        const string &header_file,
        const string &programs_file,
        const string& custom_targets_file );

private: // Helper functions

  /**
   * Utility Function which creates the correct mkdir command for a given OS
   * @param  folderName the name of the directory to make
   * @return the mkdir command
   */
  string mkdir_cmd( const string &folderName);

  /**
   * Sets the Class member variables foor the given OS and compiler.
   * TODO Check for OS/Compiler consistency, etc...
   */
  void set_system_vars();

  /**
   * Writes the default Compiler flags to cout.
   */
  void write_default_flags();

  /**
   * Writes Makefile Header to cout
   * @param header_file the file containing custom header
   */

  void write_header(string header_file);

  /**
   * Writes the Lists portion of the Makefile to cout
   * @param dirlist the directory listing.
   */
  void write_lists( DirList &dirlist );

/**
 * Writes the Targets of the makefile
 * @param dirlist the directory listing
 * @param programs_file the name of the file containing custom programs
 * @param custom_targets_file the name of the file containing custom targets
 */
  void write_main_targets( DirList &dirlist, string programs_file, string custom_targets_file );


  void write_main_target_list( const set<string> &list,string compile,string local_libs );


  void write_dependencies( DirList &dirlist );


  void write_dependency_list( const set<string> &list, DirList &dirlist );


  /**
   * Writes the Trailer portion of the makefile
   * @param dirlist the directory listing
   */
  void write_trailer( DirList &dirlist);


  set<string> products;

  ///The Compilation Commands
  string CC,CXX;
  ///The FLags used for compilation
  string DEBUGFLAG,CXXFLAGS,CFLAGS,CLIBFLAGS,CCLIBFLAGS;

  string bin_dir;      	///The BinDir variable {Bindir}
  string bin_suffix;   	///The executable suffix
  string bin_dir_name; 	///The name of the Output directory

  string Sep;           ///The Path seperator
  string null_file;     ///The NULL file for the OS
  string rm_command;    ///The OS delete command
  string rm_dir_command;///The OS delete directory command


  COMPILER Comp;        ///The Compiler to generate flags for
  OS System;			///The OS to generate commands for
  bool NoDebugFlags;	///whether or not to generate debug info
  string CompilerName;	///The Name of the Compiler
  string OSName;		///The name of the OS

}; // Write

#endif
