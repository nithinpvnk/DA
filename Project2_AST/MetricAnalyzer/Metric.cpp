#include "Metric.h"
/////////////////////////////////////////////////////////////////////
//  Metric.cpp - Find the complexity of the package.	     	   //
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
This module provides functions to find the complexity of the functions.

Build Process:
==============
Required files
- AST.h, ScopeStack.h, Parser.h, Parser.cpp

Build commands (either one)
- devenv Project3DA.sln

Maintenance History:
====================
ver 1.0 : 14 Mar 16 - First Release

*/

void Metric::print(ASTelement* elem)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(5 * indentLevel, ' ') << " ( " + elem->name + " , " + elem->type + " )";
	auto iter = elem->child.begin();
	++indentLevel;
	while (iter != elem->child.end())
	{
		print(*iter);
		++iter;
	}
	--indentLevel;
}

void Metric::preOrderTraversal(ASTelement* root)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		if (root->type == "function")
		{
			std::cout << "\n" << std::setw(15) << root->name << std::setw(15) << root->lcstart << std::setw(15) << root->lcend << std::setw(15) << root->lineCount << std::setw(15) << root->complexity;
		}
		for (int i = 0; i < root->child.size(); i++)
			preOrderTraversal(root->child.at(i));

		return;
	}
}