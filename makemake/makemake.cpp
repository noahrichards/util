/* makemake.cpp
 *
 * $Id: makemake.cpp,v 1.1 2006/06/08 02:14:38 swm Exp $
 *
 * $Log: makemake.cpp,v $
 * Revision 1.1  2006/06/08 02:14:38  swm
 * Initial revision
 *
 */

#include <string.h>
#include <iostream>
#include <cstdlib>

#include "dirlist.h"
#include "write.h"

void printHelp()
{
	cout<<"#Usage: makemake [flag1] [flag2] [...] [>Makefile]"<<endl;
	cout<<"#Notes: "<<endl;
	cout<<"#    Flag collision is not checked, previous flags are overridden."<<endl;
	cout<<"#    On windows, having sh.exe in the path may cause problems."<<endl;
	cout<<"#Flags Are:"<<endl;
	cout<<"#    -help                       Prints this help message"<<endl;
	cout<<"#"<<endl;
	cout<<"#    -header <filename>          Sets custom header file"<<endl;
	cout<<"#    -programs <filename>        Sets custom programs file"<<endl;
	cout<<"#    -custom_targets <filename>  Sets custom targets file"<<endl;
	cout<<"#"<<endl;
	cout<<"#    -no-header                  Don't use custom header file"<<endl;
	cout<<"#    -no-programs                Don't use custom programs file"<<endl;
	cout<<"#    -no-custom_targets          Don't use custom targets file"<<endl;
	cout<<"#"<<endl;
	cout<<"#    -no-debug                   Build without debug info"<<endl;
	cout<<"#"<<endl;
	cout<<"#    -win                        Use Windows CMD.exe commands"<<endl;
	cout<<"#    -linux                      Use Unix bash commands"<<endl;
	cout<<"#"<<endl;
	cout<<"#    -gcc                        Use GCC as compiler"<<endl;
	cout<<"#    -clang                      Use Clang as compiler"<<endl;
	cout<<"#    -sun                        Use SUN as compiler"<<endl;

}

int main(int argc, char **argv)
{
	string header_file("header.mak");
	string programs_file("programs.mak");
	string custom_targets_file("custom_targets.mak");


	bool noDebugFlag(false);//flag tells whether debug info is generated

	#ifdef _WIN32
	OS OpSystem(WINDOWS);//defualt to windows makefile
	#else
	OS OpSystem(LINUX); //default to linux makefile
	#endif

	//Compiler defualts to GCC
	COMPILER Comp(GCC);

	//If first Character of exe is s, then SunCompiler is default
	string programPath = argv[0];
	unsigned int i = programPath.find_last_of("/\\");
	if (i != string::npos && programPath[i + 1] == 's')
	{
		Comp=SUN;
	}

	if(strcmp(argv[1],"-help")==0 || strcmp(argv[1],"-h")==0)
	{
		printHelp();
		exit(0);
	}

	//
	// Process command line options
	//
	while (argc--, *++argv != NULL && **argv == '-')
	{
		if (strcmp(*argv, "-header") == 0 && argv[1] != NULL)
		{
			header_file = *++argv;
			argc--;
		}

		else if (strcmp(*argv,"-win") == 0)
		{
			OpSystem = WINDOWS;
		}

		else if (strcmp(*argv,"-linux") == 0)
		{
			OpSystem=LINUX;
		}

		else if (strcmp(*argv,"-gcc") == 0)
		{
			Comp = GCC;
		}

		else if (strcmp(*argv,"-sun") == 0)
		{
			Comp=SUN;
		}
		else if (strcmp(*argv,"-clang") == 0)
		{
			Comp=CLANG;
		}

		else if (strcmp(*argv, "-no-header") == 0)
		{
			header_file = "//";
		}

		else if (strcmp(*argv, "-programs") == 0 && argv[1] != NULL)
		{
			programs_file = *++argv;
			argc--;
		}

		else if (strcmp(*argv, "-no-programs") == 0)
		{
			programs_file = "//";
		}

		else if (strcmp(*argv, "-custom_targets") == 0 && argv[1] != NULL)
		{
			custom_targets_file = *++argv;
			argc--;
		}

		else if (strcmp(*argv, "-no-custom_targets") == 0)
		{
			custom_targets_file = "//";
		}

		else if (strcmp("-no-debug", *argv) == 0)
		{
			noDebugFlag = true;
		}

		else
		{

			cerr << "Invalid option: " << *argv << endl;
			if(strcmp(*argv,"-help")==0)
				cerr<<"    -help option must be used alone"<<endl;
			exit(1);
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
	Write write(dirlist, noDebugFlag, Comp, OpSystem, header_file,
			programs_file, custom_targets_file);

	return 0;
}
