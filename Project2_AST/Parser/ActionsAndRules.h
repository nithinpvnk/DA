#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp

Build commands (either one)
- devenv Project3DA.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.0 : 4 Apr 16- First Release
*/

#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "Typetable.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include <unordered_map>

class Repository  // application specific
{
	ScopeStack<ASTelement> stack;
	Scanner::Toker* p_Toker;
	ASTelement root;
	AST* tree = new AST(&stack);
public:
	Repository(Scanner::Toker* pToker)
	{
		if (stack.size() == 0)
		{
			root.name = "GlobalNameSpace";
			root.type = "root";
			root.lcstart = 0;
			stack.push(root);
			tree->setRoot(root);
		}
		p_Toker = pToker;
		table = new TypeTable<TypeTableRecord>();
	}
	ScopeStack<ASTelement>& scopeStack() { return stack; }
	AST* getTree() { return tree; }
	TypeTable<TypeTableRecord>* getTable() { return table; }
	Scanner::Toker* Toker() { return p_Toker; }
		size_t lineCount()
	{
		return (size_t)(p_Toker->currentLineCount());
	}

	void setFileName(std::string fil)
	{ 
		file = fil; 
	}
	std::string getFileName() 
	{
		return file;
	}
private:
	TypeTable<TypeTableRecord>* table;
	std::string file;
	std::unordered_map<std::string, std::vector<std::string>>* mapNamespace;
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at beginning of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ASTelement elem;
	elem.type = "scope";
		elem.name = "Start Scope";
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->scopeStack().size() == 0)
			return;
		ASTelement elem = p_Repos->scopeStack().pop();

		if (elem.type == "function")
		{
			AST* tree = p_Repos->getTree();
			elem.lcend = (p_Repos->lineCount() - 1);
			tree->setRecentElement(elem);
		}
		else if (elem.type == "namespace")
		{
			AST* tree = p_Repos->getTree();
			elem.lcend = p_Repos->lineCount();
			tree->setRecentElement(elem);
		}
		else if (elem.type == "class" || elem.type == "struct" || elem.type == "enum")
		{
			AST* tree = p_Repos->getTree();
			elem.lcend = p_Repos->lineCount();
			tree->setRecentElement(elem);
		}
		else if (elem.type == "SpclBlock")
		{
			AST* tree = p_Repos->getTree();
			elem.lcend = p_Repos->lineCount();
			tree->setRecentElement(elem);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// rule to detect the definitions of namespace, 
// class, struct and enum

class ClassNamespaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.length() - 1;
		if (tc[len] == "{")
		{
			if (tc.find("class") < tc.length() || tc.find("namespace") < tc.length()
				|| tc.find("struct") < tc.length() || tc.find("enum") < tc.length()) {
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push namespace, class, struct and 
// namespace onto scope stack

class PushClassNamespace : public IAction
{
	Repository* p_Repos;
public:
	PushClassNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		ASTelement elem;
		TypeTableRecord record;
		std::string name;
		if (pTc->find("class") < pTc->length()) {
			name = (*pTc)[pTc->find("class") + 1];
			elem.type = "class";
			record.type() = "class";
		}
		else if (pTc->find("namespace") < pTc->length()) {
			name = (*pTc)[pTc->find("namespace") + 1];
			elem.type = "namespace";
	
		}
		else if (pTc->find("struct") < pTc->length()) {
			name = (*pTc)[pTc->find("struct") + 1];
			elem.type = "struct";
			record.type() = "struct";
		}

		else if (pTc->find("enum") < pTc->length()) {
			name = (*pTc)[pTc->find("enum") + 1];
			elem.type = "enum";
			record.type() = "enum";
		}
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		AST* tree = p_Repos->getTree();
		tree->AddChildren(elem);
		p_Repos->scopeStack().push(elem);

		record.name() = name;
		record.fileName() = p_Repos->getFileName();
		record.parent() = tree->findParent(elem)->name;
		TypeTable<TypeTableRecord>* tbl = p_Repos->getTable();
		tbl->addRecord(record);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect try, do, else and else if blocks

class TryDoBlocks : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.length() - 1;
		if (tc[len] == "{")
		{
			if ((tc.find("try") < tc.length()) || (tc.find("do") < tc.length())
				|| (tc.find("else") < tc.length() && tc.find("if") >= tc.length())) {
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push try, do, else and else if onto scope stack

class PushTryDo : public IAction
{
	Repository* p_Repos;
public:
	PushTryDo(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		ASTelement elem;

		std::string name = (*pTc)[pTc->find("{") - 1];
		if (name != "try" || name != "else" || name != "do")
			name = (*pTc)[pTc->find("{") - 2];
		elem.type = "SpclBlock";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);

		AST* tree = p_Repos->getTree();
		tree->AddChildren(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect special blocks

class SpclBlockDefinition : public IRule
{
public:
	bool isSplKeyword(std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch", "else if" };

		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSplKeyword(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push special blocks onto ScopeStack

class PushSpclBlock : public IAction
{
	Repository* p_Repos;
public:
	PushSpclBlock(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{

		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		ASTelement elem;
		elem.type = "SpclBlock";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);

		AST* tree = p_Repos->getTree();
		tree->AddChildren(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		ASTelement elem;
		elem.type = "function";
		elem.name = (*pTc)[pTc->find("(") - 1];
		elem.lineCount = p_Repos->lineCount();
		AST* tree = p_Repos->getTree();
		tree->AddChildren(elem);
		p_Repos->scopeStack().push(elem);

		TypeTableRecord record;
		record.name() = (*pTc)[pTc->find("(") - 1];
		record.type() = "function";
		record.fileName() = p_Repos->getFileName();
		record.parent() = tree->findParent(elem)->name;
		TypeTable<TypeTableRecord>* tbl = p_Repos->getTable();
		tbl->addRecord(record);
	}
};

class AliasesDefinition : IRule
{
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("using") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

class PushAlias : public IAction
{
	Repository* p_Repos;
public:
	PushAlias(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		p_Repos->scopeStack().pop();
		TypeTableRecord record;
		record.name() = (*pTc)[pTc->find("using") + 1];
		record.type() = "alias";
		record.fileName() = p_Repos->getFileName();
		TypeTable<TypeTableRecord>* tbl = p_Repos->getTable();
		tbl->addRecord(record);
	}
};

class TypeDefDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "char", "int", "std::string", "long", "unsigned" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == ";")
		{
			size_t len = tc.find("typedef");
			if (len < tc.length() && !isSpecialKeyWord(tc[len + 1]))
		{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

#endif