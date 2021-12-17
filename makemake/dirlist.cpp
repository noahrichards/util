/* dirlist.cpp
 *
 * $Id: dirlist.cpp,v 1.1 2006/06/08 02:14:18 swm Exp $
 *
 * $Log: dirlist.cpp,v $
 * Revision 1.1  2006/06/08 02:14:18  swm
 * Initial revision
 *
 */

#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "dirlist.h"

using namespace std;

/* Constructor for Inserter object that just initializes fields
 * of struct
 */
FileListInserter::FileListInserter( const set<string> &f )
  : files(f), prefix(), suffix(), has_suffix(false) {}
FileListInserter::FileListInserter( const set<string> &f, string pre )
  : files(f), prefix(pre), suffix(), has_suffix(false) {}
FileListInserter::FileListInserter( const set<string> &f, string pre, string suf )
  : files(f), prefix(pre), suffix(suf), has_suffix(true) {}

/* Insertion operator for the previous inserter object. Writes a
 * space-separated list of file names in f.
 */
ostream & operator<<( ostream &os, const FileListInserter &ins ) {
  for( set<string>::const_iterator it = ins.files.begin();
       it != ins.files.end();
       ++it ) {
    if( it != ins.files.begin() ) { // don't put space before first item
      os << " ";
    }
    os << ins.prefix;
    if( ins.has_suffix ) {
      os << DirList::basename( *it ) << ins.suffix;
    } else {
      os << *it;
    }
  }
  return os;
}

/* DirList
 *
 * Read the current directory, and categorize the files found there
 * according to their suffix, saving them on the appropriate list.
 * If command line arguments were given, read that list to find
 * .C, .cpp, .c, and .s files, and ignore those found in the directory.
 * We always read .h files.
 */
DirList::DirList( int ac, char **av ){
  //
  // Read the directory and store all the files we find there,
  // EXCEPT if arguments were given, those are the only .C, .c, .cpp, and .s
  // files we're interested in, so skip any found here.
  //
  DIR *dir( opendir( "." ) );

  if( dir != 0 ){
    struct dirent *dirent;

    while( ( dirent = readdir( dir ) ) != 0 ) {
      string name( dirent->d_name );

      switch( fileType( name ) ) {
      case CPP:
        if( ac <= 0 ) {
          insert_source( hasMainFunction, name, cpp, cpp_main, cpp_other );
        }
        break;

      case C:
        if( ac <= 0 ) {
          insert_source( hasMainFunction, name, c, c_main, c_other );
        }
        break;

      case ASSEMBLY:
        if( ac <= 0 ) {
          insert_source( hasMainLabel, name, ass, ass_main, ass_other );
        }
        break;

      case H:
        h.insert( name );
        break;

      case ARCHIVE:
        archive.insert( name );
        break;

      default:;
        // nothing to do
      }

    }
    closedir( dir );
  } else {
    cerr << "Error opening current directory\n";
  }

  //
  // Copy the names from the argument list into their vector.
  //
  while( ac-- > 0 ) {
    string name( *av++ );

    switch( fileType( name ) ) {
    case CPP:
      insert_source( hasMainFunction, name, cpp, cpp_main, cpp_other );
      break;

    case C:
      insert_source( hasMainFunction, name, c, c_main, c_other );
      break;

    case ASSEMBLY:
      insert_source( hasMainLabel, name, ass, ass_main, ass_other );
      break;

    default:
      cerr << av[ -1 ] << ": ignored, not C/C++ or assembly" << endl;
    }
  }
}

/* insert_source
 *
 *  Insert a filename into the 'main' or 'other' list, depending on
 * whether it contains a main function.  Also insert it into the
 * 'both' list, too.
 */
void DirList::insert_source( bool (* mainfn)( const string &name ),
                             const string &name,
                             set<string> &both,
                             set<string> &main,
                             set<string> &other ) {
  both.insert( name );
  if( mainfn( name ) ) {
    main.insert( name );
  } else {
    other.insert( name );
  }
}

/* basename
 *
 * Return the basename (without a suffix) of name.
 */
const string DirList::basename( const string &name ) {
  size_t pos = name.rfind('.');
  if( pos == string::npos ) return name;
  return name.substr( 0, pos );
}

/* includeFiles
 *
 * Get a list of include files mentioned in file name. Cache the result
 * so successive calls do no work.
 */
set<string> DirList::includeFiles( const string &name ) {
  map<string,set<string> >::const_iterator it( _includeFiles.find( name ) );
  if( it != _includeFiles.end() ) {
    return it->second;
  } else {
    return _includeFiles[ name ] = getIncludeFiles( name );
  }
}

/*dependencies
 *
 * Do a transitive closure of include files included by file name.
 * Cache the result so successive calls do no work.
 */
set<string> DirList::dependencies( const string &name ) {
  map<string, set<string> >::iterator it = _dependencies.find( name );
  if( it != _dependencies.end() ) {
    return it->second;
  } else {
    set<string> todo( getIncludeFiles( name ) );
    set<string> closure;
    while( !todo.empty() ) {
      string s = *todo.begin();
      todo.erase( todo.begin() );
      if( closure.insert( s ).second ) {
        set<string> newfiles( getIncludeFiles( s ) );
        todo.insert( newfiles.begin(), newfiles.end() );
      }
    }
    _dependencies[ name ] = closure;
    return closure;
  }
}

/* fileType
 *
 * Return a member of the fileType enumeration corresponding to
 * the suffix in name.
 */
DirList::FileType DirList::fileType( const string &name ) const {
  if( name[ 0 ] == '.' ) return OTHER;
  size_t pos = name.rfind('.');
  if( pos == string::npos ) return OTHER;
  string suffix( name.substr( pos+1, name.size() ) );
  FileType ft = OTHER;
  if( suffix == "C" || suffix == "cpp" || suffix == "cc" ) ft = CPP;
  if( suffix == "c" ) ft = C;
  if( suffix == "s" ) ft = ASSEMBLY;
  if( suffix == "H" || suffix == "h" ) ft =  H;
  if( suffix == "a" ) ft = ARCHIVE;
  if( ft == OTHER ) return OTHER;
  if( name.find('#') != string::npos ) {
    cerr << "Warning: ignoring file whose name contains '#': "
         << name << endl;
    return OTHER;
  }
  return ft;
}

/* matchString
 *
 * Matches a given string or stops on first character of mismatch. All
 * matched characters are passed in the stream. ch will be set to the
 * first unmatched character or the next character if the string is
 * completely matched.
 */
bool DirList::matchString( ifstream &in, char &ch, const string &s ) {
  int n = s.size();
  for( int i = 0; i < n && in; i++, in.get( ch ) ) {
    if( s[i] != ch ) {
      return false;
    }
  }
  return !in.fail(); // true if stream still good else false for no match
}

/* skipComments
 *
 * Skips a possible comment.
 *
 * May absorb a '/' and return false. ch will be set to the character
 * following the comment or following a '/' that does not start a comment.
*/
bool DirList::skipComments( ifstream &in, char &ch ) {
  if( ch == '/' && in ) {
    if( in.get( ch ) && ch == '/' ) {
      while( in.get( ch ) && ch != '\n' ) {/* empty */}
      in.get( ch ); // pass end-of-line
      return true;
    } else if( ch == '*' ) {
      if( in.get( ch ) ) {
        do {
          while( ch != '*' && in.get( ch ) ) {/* empty */}
          while( ch == '*' && in.get( ch ) ) {/* empty */}
        } while( ch != '/' && in );
      }
      return true;
    } else {
      return false;
    }
  }
  return false;
}

/* skipWhitespace
 *
 * Skips possible whitespace. ch will be set to the first non-whitespace
 * character.
 */
bool DirList::skipWhitespace( ifstream &in, char &ch ) {
  bool skipped = false;
  while( isspace( ch ) && in ) {
    in.get( ch );
    skipped = true;
  }
  return skipped;
}

/* getString
 *
 * Returns quoted string.
 *
 * Assumes that ch contains the quote character. The next character read
 * from the stream is the first character of the quoted string. ch is set
 * after the closing quote character. An empty string is returned if a
 * newline or end-of-file is encountered.
 */
string DirList::getString( ifstream &in, char &ch ) {
  char quote = ch;
  ostringstream os;
  while( in.get( ch ) && ch != quote && ch != '\n' ) {
    os << ch;
  }
  if( ch == quote ) {
    in.get( ch ); // pass the closing quote character
    return os.str();
  } else {
    return string(); // return an empty string on failure
  }
}

/* hasMainFunction
 *
 * Scan a file to see if it contains a main function.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws) "int" ws+ "main" ws* "("
 */
bool DirList::hasMainFunction( const string &name ) {
  ifstream in( name.c_str() );
  char ch;

  in.get( ch );
  while( in ) {
    while( skipWhitespace( in, ch ) || skipComments( in, ch ) ) {/* empty */}
    if ( matchString( in, ch, "int" ) ) {
      if( isspace( ch ) ) {
        while( skipWhitespace( in, ch ) ) {/* empty */}
        if( matchString( in, ch, "main" ) ) {
          while( skipWhitespace( in, ch ) ) {/* empty */}
          if( matchString( in, ch, "(" ) ) {
            return true;
          }
        }
      }
    }
    in.get( ch );
  }
  return false;
}

/* hasMainLabel
 *
 * Scan a file to see if it contains a main label.
 * This is only a partial job, because to really do it right
 * takes about the same amount of work as compiling the file.
 *
 * 1) ignore C++ style comments,
 * 2) ignore C style comments (fails if the file has strings
 *    that contain / * or * /  ),
 * 3) then look for the pattern:
 *  (^|ws) "main" ws* ":"
 */
bool DirList::hasMainLabel( const string &name ) {
  ifstream in( name.c_str() );
  char ch;

  in.get( ch );
  while( in ) {
    while( skipWhitespace( in, ch ) || skipComments( in, ch ) ) {/* empty */}
    if ( matchString( in, ch, "main" ) ) {
      while( skipWhitespace( in, ch ) ) {/* empty */}
      if( matchString( in, ch, ":" ) ) {
        return true;
      }
    } else {
      in.get( ch );
    }
  }
  return false;
}

/* getIncludeFiles
 *
 * Read the file, looking for this pattern:
 * ^(ws*)#(ws*)include(ws*)"name"
 * For each entry found, add the name to the filelist.
 */
set<string> DirList::getIncludeFiles( const string &name ) {
  ifstream in( name.c_str() );
  set<string> list;
  ostringstream os;
  char ch;

  in.get( ch );
  while( in ) { // at beginning of line
    skipWhitespace( in, ch );
    if( matchString( in, ch, "#" ) ) {
      skipWhitespace( in, ch );
      if( matchString( in, ch, "include" ) ) {
        skipWhitespace( in, ch );
        if( ch == '"' ) {
          //in.get( ch );
          string name = getString( in, ch );
          if( name.size() != 0 ) {
            list.insert( name );
          }
        }
      }
    }
    while( ch != '\n' && in.get( ch ) ) {/* empty */}
    in.get(ch);
  }
  return list;
}
