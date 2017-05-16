#ifndef TESTSUITE_H
#define TESTSUITE_H

/////////////////////////////////////////////////////////////////////
//  TestSuite.h:	 Tests the FileManager and Parser components   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
* Metric Executive provides functions to test the File Manager and
* Parser components. It takes path and patterns as inputs from console.
* Retrives all the files that match the given pattern and passes
* them to Parser for building an AST

Public Interface:
=================
testFileMgr(path,patterns) - tests the FileMngr component
testParser(string file)	- tests the Parser component including AST

Build Process:
==============
Required files
- FileMngr.h, FileMngr.cpp, MtrAnalysis.h, MtrAnalysis.cpp, Parser.h, Parser.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.h, AST.h, AST.cpp

Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 4 Apr 16 - first release

*/


#include<vector>
#include <set>
#include <list>
#include <map>
#include <string>
#include <windows.h>
#include <iostream>
#include "../FileManager/FileMgr.h"
#include "../MetricAnalyzer/Metric.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Typetable.h"

class TestExec
{
public:
	void testFileMgr(std::string path, std::vector<std::string> patterns);
	void testParser(std::string file);
};


#endif 
