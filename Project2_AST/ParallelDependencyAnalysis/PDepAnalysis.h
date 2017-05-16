#ifndef PDEPANALYSIS_H
#define PDEPANALYSIS_H

/////////////////////////////////////////////////////////////////////
//  PDepAnalysis.h - runs dependency analysis asynchronously	   //
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
* provides callable objects to run the dependency analysis
* asynchronously using thread pool.

Public Interface:
=================
* getLamda() - provides lamdas and enqueues work item
* trimPath() - trim path to file name
* end()		 - adds end condition for file manager

Build Process:
==============
Required files
- ThreadPool.h, Tokenizer.h, Tokenizer.cpp, TypeTable.h
- FileMgr.h, DependencyExec.h, DependencyExec.cpp

Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First release

*/


#include "../ThreadPool/QWorkedItem.h"
#include "../DependencyAnalysis/DepAnalysis.h"
#include"../Parser/TypeTable.h"
#include"../FileManager/FileMgr.h"
#include<fstream>


class PDepAnalysis
{
public:
	PDepAnalysis(TypeTable<TypeTableRecord> MergedTypeTable1, std::string path, std::vector<std::string> patterns, ProcessWorkItem<std::string> &tp) {
		MergedTypeTable = MergedTypeTable1;
		_path = path;
		_patterns = patterns;
		threadp = &tp;
		_blockQ = new BlockingQueue<std::string>();
	}
	void beginwork1();
	void end() { _blockQ->enQ("Completion of the file"); }
private:
	TypeTable<TypeTableRecord> MergedTypeTable;
	BlockingQueue<std::string>* _blockQ;
	std::vector<std::string> _patterns;
	std::string _path;
	std::string trimPath(std::string fullpath);
	ProcessWorkItem<std::string>* threadp = new ProcessWorkItem<std::string>(3);

};

#endif

