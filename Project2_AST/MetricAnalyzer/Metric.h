#ifndef METRICANALYSIS_H
#define METRICANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  Metric.h - Find the complexity of the package.				   //
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

Public Interface:
=================
void printAST(ASTelement* elem) - This function is used to print the AST structure
void FuncDetail(ASTelement* elem) - This function is used to print the complexity of the functions in detail.

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

#include<iomanip>
#include "../AST/AST.h"

class Metric
{
public:
	void print(ASTelement* elem);
	void preOrderTraversal(ASTelement* elem);
};

#endif 