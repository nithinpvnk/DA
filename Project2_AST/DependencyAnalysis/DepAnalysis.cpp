#include "DepAnalysis.h"
/////////////////////////////////////////////////////////////////////
//  DepAnalysis.cpp - checks for dependency on files			   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////

/*

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

DepAnalysis::DepAnalysis(Toker* pToker, TypeTable<TypeTableRecord>* Ttbl, std::string file) : _pToker(pToker), _typeTbl(Ttbl), _file1(file)
{//initialise
	mapDep = new std::unordered_map<std::string, std::vector<std::string>>();
}

std::string DepAnalysis::getTok()//collection of tokens
{
	if (_pToker == nullptr)
		throw(std::logic_error("There is no Toker reference"));
	return _pToker->getTok();

}

void DepAnalysis::compare(std::string tok)
{
	for (auto type : *_typeTbl)
	{
		if (tok.compare(type.name()) == 0 && tok.compare("main") != 0)
		{
			if (_file1.compare(type.fileName()) != 0)
			{
				std::string fileName = type.fileName();
				std::vector<std::string> vec;

				if (!mapDep->count(_file1))
				{
					vec.push_back(fileName);
					mapDep->insert(std::make_pair(_file1, vec));
				}
				else
				{
					vec = mapDep->at(_file1);

					if (std::find(vec.begin(), vec.end(), fileName) != vec.end())
						mapDep->at(_file1).push_back(fileName);

						/*break;
					else
						mapDep->at(_file1).push_back(fileName);*/
				}
			}
			/*else
				break;*/
		}
	}
}

void DepAnalysis::dependency()
{
	while (true)
	{
		std::string token = getTok();

		if (token == "")
			break;
		else
			compare(token);
	}
}

void DepAnalysis::showDep()
{
	for (auto rec : *mapDep)
	{
		std::cout << rec.first << std::setw(20) << " has dependancy on ";
		for (auto vc : rec.second)
			std::cout << vc << "    ";
		std::cout << "\n";
	}
}

#ifdef TEST_DEPANALYSIS

void main()
{
	std::cout << "Dependencies :" << "\n";
	std::cout << "================" << "\n";
	Toker tok;
	std::fstream in(filepath);

	tok.attach(&in);

	DepAnalysis dep(&tok, &MergedTypeTable, trimPath(filepath));
	dep.dependency();

	dep.showDep();
}
#endif // TEST_DEPANALYSIS
