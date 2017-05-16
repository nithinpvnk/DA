#ifndef PDTESTEXEC_H
#define PDTESTEXEC_H
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
Module Operations:
==================
* This package is the Test executive for Project 3 Parallel Dependency

Public Interface:
=================
* testFile() - sends the arguments to task package

Build Process:
==============
Required files
- Task.h, Task.cpp

Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First release

*/

#include "../Task/Task.h"
#include <vector>

class DepAnalysisExec
{
public:
	void testFile(std::string path, std::vector<std::string> patterns);
};

#endif
