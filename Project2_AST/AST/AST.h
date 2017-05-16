#ifndef AST_H
#define AST_H
/////////////////////////////////////////////////////////////////////
//  AST.h - Provides the required function for the Tree.	       //
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
This module provides functions to build an Abstract Syntax Tree (AST).

Public Interface:
=================
void AddChild(element node) - This function is used to add a child to the existing node while take the scopestack topmost element as the parent.
element* RootElement() - This function is used to return the root of the AST.
void setRoot(element) - This function is used to add the root in the tree.
element* findParent(element) - This function is used to get the top element of the ScopeStack.
void setRecent(element node) - This function is used to change the current node accordingly with ScopeStack.

Build Process:
==============
Required files
- ASTNode.h, ScopeStack.h

Build commands:
==============
- devenv Project3DA.sln

Maintenance History:
====================
ver 1.0 : 7 Apr 16 - First Release

*/

#include<iostream>
#include "ASTNode.h"

class AST 
{
private:
	ASTelement* root;
	ASTelement* recent;
	ScopeStack<ASTelement>* scopeStack;
public:
	
	AST(ScopeStack<ASTelement>* scopeStack);
	void AddChildren(ASTelement node);
	ASTelement* Root();
	void setRoot(ASTelement);
	ASTelement* findParent(ASTelement);
	void setRecentElement(ASTelement node);
};

#endif