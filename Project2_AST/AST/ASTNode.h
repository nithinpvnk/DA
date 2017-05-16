#ifndef ASTNODE_H
#define ASTNODE_H

/////////////////////////////////////////////////////////////////////
//  ASTNode.h - Provides the required Nodes for the Tree.          //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., Syracuse University	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds an Abstract Syntax Tree (AST).

Public Interface:
=================
ASTelement - This function is a constructor used to carry the default values of the required.

Build Process:
==============
Required files
- ScopeStack.h, AST.h, AST.cpp

Build commands:
==============
- devenv Project3DA.sln

Maintenance History:
====================
ver 1.0 : 4 Apr 16 - First Release

*/

#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include "../ScopeStack/ScopeStack.h"

struct ASTelement
{
	ASTelement* parent;
	std::vector<ASTelement*> child;
	std::string type;
	std::string name;
	size_t lineCount;
	size_t lcstart;
	size_t lcend;
	size_t complexity;

	ASTelement()
	{
		lcstart = 0;
		lcend = 0;
		lineCount = 0;
		complexity = 1;
		parent = NULL;
	}
};

#endif