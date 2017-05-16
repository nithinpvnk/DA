#include "PDTestExec.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
//  PDepTestExec.h - runs dependency analysis asynchronously	   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////

/*
Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First release

*/


void DepAnalysisExec::testFile(std::string path, std::vector<std::string> patterns)
{
	std::cout << "Path Given : " << "\n";
	std::cout << "============" << "\n";
	std::cout << path << "\n";

	std::cout << "\n Given Patterns : ";
	std::cout << "\n" << "================" << "\n \n";
	for (auto str : patterns)
		std::cout << str << "\t";
	std::cout  << "\n";

	Task tasks(path, patterns);
}


int main(int argc, char* argv[])
{
	std::vector<std::string> patterns;
	DepAnalysisExec dExec;
	std::string path;
	if (argc > 1)
	{
		path = argv[1];

		for (int i = 2; i < argc; i++)
		{
			patterns.push_back(argv[i]);
		}
		//Task tasks(path,patterns);
		dExec.testFile(argv[1], patterns);
	}
	else
		std::cout << "Wrong path or patterns.! ";

	return 0;
}


