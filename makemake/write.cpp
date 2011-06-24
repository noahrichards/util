/* write.cpp
 *
 * $Id: write.cpp,v 1.3 2006/06/08 16:13:53 swm Exp $
 *
 * $Log: write.cpp,v $
 * Revision 1.3  2006/06/08 16:13:53  swm
 * removed CR's before NL's
 *
 * Revision 1.2  2006/06/08 02:47:34  swm
 * Added support for windows with .exe extensions on executables
 *
 * Revision 1.1  2006/06/08 02:15:02  swm
 * Initial revision
 *
 */

#include <sys/file.h>
#include <sys/types.h>
#ifdef        sparc
#    include <sys/timeb.h>
#endif
#include <unistd.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "write.h"

const string bin_dir("${BINDIR}/");
#ifdef WINDOWS
const string bin_suffix(".exe");
#else
const string bin_suffix("");
#endif


// for debug
// #define WINDOWS
using namespace std;

Write::Write( DirList &dirlist, bool noDebugFlag, bool useGCC,
              const string &header_file, const string &programs_file,
              const string& custom_targets_file) {
  write_header(noDebugFlag, useGCC, header_file);
  write_lists( dirlist );
  write_main_targets( dirlist, programs_file, custom_targets_file );
  write_dependencies( dirlist );
  write_trailer( dirlist, useGCC );
}

void Write::write_header(bool noDebugFlag, bool useGCC, string header_file) {
  ifstream in;
  time_t now( time( 0 ) );

  //
  // Open the header file
  //
  if( header_file == "//" ){
    //
    // They specifically said not to use one.
    //
    in.clear( ios::badbit | ios::failbit );
  } else {
    //
    // Open the one they said to use.
    //
    in.open( header_file.c_str() );
  }

  //
  // Write the header
  //
  cout << "#\n";
  if(useGCC) {
    cout << "# Created by makemake (";
  } else {
    cout << "# Created by smakemake (" ;
  }
  cout << "Darwin " __DATE__ ") on " << ctime( &now );
  cout << "#\n";
  cout << "\n";
  cout << "#\n";
  cout << "# Definitions\n";
  cout << "#\n";
  cout << "\n";
  cout << ".SUFFIXES:\n";
  cout << ".SUFFIXES:\t.a .o .c .C .cpp .s\n";
  cout << ".c.o:\n";
  cout << "\t\t$(COMPILE.c) $<\n";
  cout << ".C.o:\n";
  cout << "\t\t$(COMPILE.cc) $<\n";
  cout << ".cpp.o:\n";
  cout << "\t\t$(COMPILE.cc) $<\n";
  cout << ".s.o:\n";
  cout << "\t\t$(COMPILE.cc) $<\n";
  cout << ".c.a:\n";
  cout << "\t\t$(COMPILE.c) -o $% $<\n";
  cout << "\t\t$(AR) $(ARFLAGS) $@ $%\n";
  cout << "\t\t$(RM) $%\n";
  cout << ".C.a:\n";
  cout << "\t\t$(COMPILE.cc) -o $% $<\n";
  cout << "\t\t$(AR) $(ARFLAGS) $@ $%\n";
  cout << "\t\t$(RM) $%\n";
  cout << ".cpp.a:\n";
  cout << "\t\t$(COMPILE.cc) -o $% $<\n";
  cout << "\t\t$(AR) $(ARFLAGS) $@ $%\n";
  cout << "\t\t$(RM) $%\n";
  cout << "\n";
  if(useGCC) {
    cout << "CC =\t\tgcc\n";
    cout << "CXX =\t\tg++\n";
  } else {
    cout << "CC =\t\tcc\n";
    cout << "CXX =\t\tCC\n";
  }
  cout << "\n";
  cout << "RM = rm -f\n";
  cout << "AR = ar\n";
  cout << "LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)\n";
  cout << "LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)\n";
  cout << "COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c\n";
  cout << "COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c\n";
  cout << '\n';
  if(!noDebugFlag){
    cout << "########## Default flags (redefine these with a header.mak file if desired)\n";
    if(useGCC) {
      cout << "CXXFLAGS =\t-ggdb -Wall -ansi -pedantic\n";
      cout << "CFLAGS =\t-ggdb -Wall -ansi -pedantic\n";
      cout << "BINDIR =./bin\n";
    } else {
      cout << "CXXFLAGS =\t-g -xildoff -xsb\n";
      cout << "CFLAGS =\t-g\n";
      cout << "BINDIR =./bin\n";
    }
    cout << "CLIBFLAGS =\t-lm\n";
    cout << "CCLIBFLAGS =\t\n";
    cout << "########## End of default flags\n";
    cout << '\n';
  } else {
    cout << "########## Default flags (redefine these with a header.mak file if desired)\n";
    if(useGCC) {
      cout << "CXXFLAGS =\t-Wall -ansi -pedantic\n";
      cout << "CFLAGS =\t-Wall -ansi -pedantic\n";
      cout << "BINDIR =./bin\n";
    } else {
      cout << "CXXFLAGS =\t-xildoff -xsb\n";
      cout << "CFLAGS =\t\n";
      cout << "BINDIR =./bin\n";
    }
    cout << "LIBFLAGS =\t-lm\n";
    cout << "########## End of default flags\n";
    
    cout << '\n';
  }
  if( in ){
    cout << "########## Flags from " << header_file << "\n\n";
    copy(istreambuf_iterator<char>(in),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(cout));
    cout << '\n'; // in case no end-of-line at end of file
    cout << "########## End of flags from " << header_file << "\n\n";
  } 
  
  cout << '\n';
}

void Write::write_lists( DirList &dirlist ){
  cout << "CPP_FILES =\t" << FileListInserter(dirlist.cpp) << '\n';
  cout << "C_FILES =\t" << FileListInserter(dirlist.c) << '\n';
  cout << "S_FILES =\t" << FileListInserter(dirlist.ass) << '\n';
  cout << "H_FILES =\t" << FileListInserter(dirlist.h) << '\n';
  cout << "SOURCEFILES =\t$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)\n";
  cout << ".PRECIOUS:\t$(SOURCEFILES)\n";

  cout << "OBJFILES =\t";
  cout << FileListInserter( dirlist.cpp_other, "", ".o" );
  if( !dirlist.cpp_other.empty() ) {
    cout << " ";
  }
  cout << FileListInserter( dirlist.c_other, "", ".o" );
  if( !dirlist.c_other.empty() ) {
    cout << " ";
  }
  cout << FileListInserter( dirlist.ass_other, "", ".o" );
  cout << '\n';

  if( !dirlist.archive.empty() ){
    cout << "LOCAL_LIBS =\t" << FileListInserter( dirlist.archive ) << '\n';
  }
  cout << '\n';
}

void Write::write_main_targets( DirList &dirlist, string programs_file, string custom_targets_file ){
  string local_libs( !dirlist.archive.empty()
                          ? " $(LOCAL_LIBS)"
                          : "" );
  ifstream in;
  ifstream custom_targets;

  if( dirlist.cpp_main.empty() && dirlist.c_main.empty()
      && dirlist.ass_main.empty() ) {
    cerr << "Warning: no main program(s) found\n";
  }
  //
  // Open the programs file
  //
  if( programs_file == "//" ){
    //
    // They specifically said not to use one.
    //
    in.clear( ios::badbit | ios::failbit );
  } else {
    //
    // Open the one they said to use.
    //
    in.open( programs_file.c_str() );
  }
  if (custom_targets_file == "//" ){
    custom_targets.clear( ios::badbit | ios::failbit );
  } else {
    custom_targets.open( custom_targets_file.c_str() );
  }

  vector<string> lines;
  istreambuf_iterator<char> init(in), eof;

  if(in) { // read lines of "programs.mak"
    while(in && init != eof) {
      ostringstream ss;
      ostreambuf_iterator<char> ssit(ss);
      while(in && init != eof && *init != '\n' && *init != '\r') {
        *ssit++ = *init++;
      }
      while(in && init != eof && (*init == '\n' || *init == '\r')) {
        init++; // skip eol
      }
      string line = ss.str();
      string::iterator colon = find(line.begin(), line.end(), ':');
      if(colon == line.end()
         || find(line.begin(), colon, ' ') != colon
         || find(colon + 1, line.end(), ':') != line.end()) {
        cerr << "error in format of file : " << programs_file << '\n';
        cerr << line << '\n';
      } else {
        lines.push_back(string(line.begin(), colon) + bin_suffix
                        + string(colon, line.end()));
        products.insert(string(line.begin(), colon) + bin_suffix);
      }
    }
    
    //
    // Executable targets
    //
    cout << "#\n"
         << "# Main targets\n"
         << "#\n\n"
         << "all:";
    for(vector<string>::iterator lit = lines.begin();
        lit != lines.end();
        ++lit) {
      string line = *lit;
      string::iterator colon = find(line.begin(), line.end(), ':');
      if(colon != line.end()) {
        cout << " " << bin_dir << string(line.begin(), colon);
      }
    }

    cout << "\n\n";
    
    for(vector<string>::iterator lit = lines.begin();
        lit != lines.end();
        ++lit) {
      string line = *lit;
      string::iterator colon = find(line.begin(), line.end(), ':');
      bool cpplink = false;
      istringstream iss(string(colon + 1, line.end()));
      string name;
      while(iss >> name) {
        string bname = DirList::basename(name);
        for(set<string>::iterator nit = dirlist.cpp.begin();
            nit != dirlist.cpp.end();
            ++nit) {
          if(DirList::basename(*nit) == bname) {
            cpplink = true;
            goto cpplinkdone;
          }
        }
      }
    cpplinkdone:
      cout << line << '\n';
      if(cpplink) {
        cout << '\t' << "$(CXX) $(CXXFLAGS) -o "
             << string(line.begin(), colon) << ' '
             << string(colon+1, line.end()) << local_libs << " $(CCLIBFLAGS)"
             << "\n\n";
      } else {
        cout << '\t' << "$(CC) $(CFLAGS) -o "
             << string(line.begin(), colon) << ' '
             << string(colon+1, line.end()) << local_libs << " $(CLIBFLAGS)"
             << "\n\n";
      }
    }
  } else {
    //
    // Executable targets
    //
    cout << "#\n"
         << "# Main targets\n"
         << "#\n\n"
         << "all:\t";

    cout << FileListInserter( dirlist.cpp_main, bin_dir, bin_suffix );
    if( !dirlist.cpp_main.empty() ) {
      cout << " ";
    }
    if (!dirlist.c_main.empty()) {
      cout << FileListInserter( dirlist.c_main, bin_dir, bin_suffix );
      cout << " ";
    }
    cout << FileListInserter( dirlist.ass_main, bin_dir, bin_suffix );
    cout << "\n\n";
    write_main_target_list( dirlist.cpp_main,
                            "$(CXX) $(CXXFLAGS)",
                            local_libs + " $(CCLIBFLAGS)" );
    write_main_target_list( dirlist.c_main,
                            "$(CC) $(CFLAGS)" ,
                            local_libs + " $(CLIBFLAGS)");
    write_main_target_list( dirlist.ass_main,
                            "$(CC) $(CFLAGS)" ,
                            local_libs + " $(CLIBFLAGS)");
  } 
  // Write custom targets
  if( custom_targets ){
    cout << "########## Custom targets from " << custom_targets_file << "\n\n";
    copy(istreambuf_iterator<char>(custom_targets),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(cout));
    cout << '\n'; // in case no end-of-line at end of file
    cout << "########## End targets from " << custom_targets_file << "\n\n";
  } 
}

void Write::write_main_target_list( const set<string> &list,
                                    string compile,
                                    string local_libs ) {
  for( set<string>::const_iterator it = list.begin();
       it != list.end();
       ++it ) {
    string basename( DirList::basename( *it ) );

    cout << bin_dir << basename << bin_suffix << ":\t"
         << basename << ".o $(OBJFILES)" << '\n';
    cout << "\t@mkdir -p " << bin_dir << "\n";

    cout << '\t' << compile << " -o "
         << bin_dir << basename << bin_suffix << ' '
         << basename << ".o $(OBJFILES)" << local_libs
         << "\n\n";
  }
}

void Write::write_dependencies( DirList &dirlist ){
  cout << "#\n"
       << "# Dependencies\n"
       << "#\n\n";

  write_dependency_list( dirlist.cpp, dirlist );
  write_dependency_list( dirlist.c, dirlist );
  cout << '\n';
}

void Write::write_dependency_list( const set<string> &list,
                                   DirList &dirlist ){
  for( set<string>::const_iterator nameit = list.begin();
       nameit != list.end();
       ++nameit ) {
    string name( *nameit );
    set<string> includes( dirlist.includeFiles( name ) );
    set<string> dependencies;

    for( set<string>::iterator it = includes.begin();
         it != includes.end();
         ++it ) {

      if( false ){
        cerr << "Warning: " << name << " not found\n";
        continue;
      }

      set<string> ilist( dirlist.dependencies( *it ) );

      dependencies.insert( *it );
      dependencies.insert( ilist.begin(), ilist.end() );
    }

    cout << DirList::basename( name ) << ".o:\t" 
         << FileListInserter(dependencies) << '\n';
  }
}

void Write::write_trailer( DirList &dirlist, bool useGCC ){
  cout << "#\n";
  cout << "# Housekeeping\n";
  cout << "#\n";
  cout << "\n";
  cout << "Archive:\tarchive.tgz\n";
  cout << "\n";
  cout << "archive.tgz:\t$(SOURCEFILES) Makefile\n";
  cout << "\ttar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz\n";
  cout << '\n';

  cout << "clean:\n"
       << "\t-/bin/rm $(OBJFILES)";
  if( !dirlist.cpp_main.empty() ){
    cout << " " << FileListInserter( dirlist.cpp_main, "", ".o" );
  }
  if( !dirlist.c_main.empty() ){
    cout << " " << FileListInserter( dirlist.c_main, "", ".o" );
  }
  if( !dirlist.ass_main.empty() ){
    cout << " " << FileListInserter( dirlist.ass_main, "", ".o" );
  }
  if(useGCC) {
    cout << " core 2> /dev/null\n";
  } else {
    cout << " ptrepository SunWS_cache .sb ii_files core 2> /dev/null\n";
  }
  cout << '\n';
  cout << "realclean:        clean\n";
  cout << "\t-/bin/rm -rf ";
  if(products.empty()) {
    cout << FileListInserter( dirlist.cpp_main, bin_dir, bin_suffix );
    if( !dirlist.cpp_main.empty() ) {
      cout << " ";
    }
    cout << FileListInserter( dirlist.c_main, bin_dir, bin_suffix );
    if( !dirlist.c_main.empty() ) {
      cout << " ";
    }
    cout << FileListInserter( dirlist.ass_main, bin_dir, bin_suffix );
    if( !dirlist.ass_main.empty() ) {
      cout << " ";
    }
  } else {
    for(set<string>::iterator it = products.begin();
        it != products.end();
        ++it) {
      cout << *it << ' ';
    }
  }
  cout << endl; // to flush buffer
}
