#include "AST.h"

/////////////////////////////////////////////////////////////////////
//  AST.cpp  - Provides the required function for Tree.		       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////

/*
Module Operations :
== == == == == == == == ==
This module provides functions to build an Abstract Syntax Tree(AST).

Build Process :
== == == == == == ==
Required files
- ASTNode.h, AST.h, ScopeStack.h

Build commands :
== == == == == == ==
-devenv Project3DA.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 7 Apr 16 - First Release

*/


void AST::AddChildren(ASTelement node1) // adds child node to the AST taking scopestack top element as parent
{
	ASTelement* node = new ASTelement();
	node->name = node1.name;
	node->type = node1.type;
	node->lineCount = node1.lineCount;
	node->lcstart = (node1.lineCount - 1);

	if (root == NULL)
	{
		root = node;
		recent = node;
		return;
	}

	ASTelement* parent1 = recent;

	if (parent1 != NULL)
	{
		if (node->type == "SplBlock")
			node->complexity = 1;

		parent1->child.push_back(node);
		node->parent = parent1;
		recent = node;
	}
	return;
}

ASTelement* AST::findParent(ASTelement elem) // Fetches the top element of ScopeStack 
{
	if (recent != NULL)
	{
		std::string name = elem.name;
		std::string type = elem.type;

		while (true)
		{
			if (recent == NULL)
				return root;
			else if (name == recent->name && type == recent->type)
				return recent;
			else
				recent = recent->parent;
		}
		return NULL;
	}
	return NULL;
}

AST::AST(ScopeStack<ASTelement>* scopeStack1)
{
	scopeStack = scopeStack1;
	root = NULL;
	recent = NULL;
}

ASTelement* AST::Root() // returns the root of AST
{
	return root;
}

void AST::setRoot(ASTelement node1) // root is added to begin building the tree 
{
	ASTelement* node = new ASTelement();
	node->name = node1.name;
	node->type = node1.type;
	node->lineCount = node1.lineCount;
	node->lcstart = node1.lineCount;
	root = node;
	recent = root;
	return;
}

void AST::setRecentElement(ASTelement node) // changes the current node accordingly with ScopeStack
{
	if (recent != NULL && recent->parent != NULL)
	{
		recent->lcend = node.lcend;
		recent->lineCount = recent->lcend - recent->lcstart + 1;
		(recent->parent)->complexity = (recent->parent)->complexity + recent->complexity;
		recent = recent->parent;
	}
}

#ifdef TEST_AST

void main()
{
	ScopeStack<ASTelement> stack;
	AST* tree = new AST(&stack);

	ASTelement elem;
	elem.name = "ASTelement";
	elem.type = "struct";
	elem.lcstart = 43;
	elem.lcend = 62;

	tree->AddChildren(elem);

	ASTelement elem;
	elem.name = "AST";
	elem.type = "class";
	elem.lcstart = 64;
	elem.lcend = 80;

	tree->setRecentElement(elem);
	tree->AddChildren(elem);

	std::cout << elem.name << "\t" << elem.type << "\t" << elem.lcstart << "\t" << elem.lcend << elem.lineCount << "\t" << elem.parent;

	std::cout << elem.name << "\t" << elem.type << "\t" << elem.lcstart << "\t" << elem.lcend << elem.lineCount << "\t" << elem.parent;
}
#endif // TEST_AST
