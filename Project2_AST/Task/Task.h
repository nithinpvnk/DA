#ifndef TASK_H
#define TASK_H

///////////////////////////////////////////////////////////////////////
// Task.h  -provides callable obj to run Type analysis asynchronously//
//  ver 1.0                                                          //
//  Language:      Visual C++ 2015, SP1                              //
//  Platform:      MacBook Pro, Windows 7 Professional	             //
//  Application:   Project for CSE687 Pr3, Sp16	                     //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	     //
//                 vputchak@syr.edu								     //
///////////////////////////////////////////////////////////////////////


/*
Module Operations:
==================
* creates 'n' threads and provides callable objects to run Type Analysis
* asynchronously. Provides methods to print them.

Public Interface:
=================
* beginwork()			- provides lamdas and enqueues work item
* end()					- adds end condition for file manager

Build Process:
==============
Required files
- BlockingQ.h, ThreadPool.h, ConfigureParser.h, ConfigureParser.cpp
- TypeTable.h, FileMgr.h, DependencyExec.h, DependencyExec.cpp
- ParallelDA.h

Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First Release

*/

#include "../ThreadPool/BlockingQ.h"
#include "../ThreadPool/QWorkedItem.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/TypeTable.h"
#include "../FileManager/FileMgr.h"
#include "../ParallelDependencyAnalysis/PDepAnalysis.h"

#include<vector>

class Task
{
public:
	Task(std::string, std::vector<std::string>);
	void beginwork();
	void end() { _blockQ->enQ("Completion of the file"); }
	//TypeTable<TypeTableRecord> MergedTypeTable;
	void filepatternsearch();
	void parserfile(std::string file);
	void mergertable();

private:
	BlockingQueue<std::string>* _blockQ;
	std::vector<std::string> _patterns;
	std::string _path;
	std::vector<TypeTable<TypeTableRecord>*> TypeTables;
	TypeTable<TypeTableRecord> MergedTypeTable;
	std::string trimPath(std::string fullpath);
	
};

#endif 