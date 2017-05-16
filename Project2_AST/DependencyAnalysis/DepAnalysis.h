#ifndef DEPANALYSIS_H
#define DEPANALYSIS_H

/////////////////////////////////////////////////////////////////////
//  DepAnalysis.h - checks for dependency on files				   //
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
* Tokenizes the given file and matches the tokens
* and its namespace with type table.

Public Interface:
=================
* getTok()		- returns a token using tokenizer
* compare()		- matches token with the type table
* dependency()	- end conditions for tokenizing the file
* showDep()		- prints the dependencies to console

Build Process:
==============
Required files
- Tokenizer.h, Tokenizer.cpp, TypeTable.h

Build commands:
==============
- devenv Project3DA.sln

Maintenance History:
====================
ver 1.0 : 7 Apr 16 - First Release

*/

#include <vector>
#include <unordered_map>
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/TypeTable.h"

using namespace Scanner;

class DepAnalysis
{
public:
	DepAnalysis(Toker* pToker, TypeTable<TypeTableRecord>* Ttbl, std::string file);
	DepAnalysis(const DepAnalysis&) = delete;
	DepAnalysis& operator=(const DepAnalysis&) = delete;
	std::string getTok();
	void compare(std::string tok);
	void dependency();
	void showDep();
	std::unordered_map<std::string, std::vector<std::string>> getmap() 
	{ return *mapDep; }
private:
	std::vector<std::string> _tokens;
	Toker* _pToker;
	TypeTable<TypeTableRecord>* _typeTbl;
	std::unordered_map<std::string, std::vector<std::string>>* mapDep;
	std::string _file1;
};

#endif