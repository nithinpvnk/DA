///////////////////////////////////////////////////////////////////////
// MT4Q1.cpp - TypeTable                                             //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2016 //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../Utilities/Utilities.h"
#include "../AST/AST.h"
#include "../Parser/ActionsAndRules.h"
#include "../TestSuite/TestSuite.h"
/*
*  The record class below could be simplified to a simple struct with
*  all public fields, assuming that our Table class doesn't need to
*  expose that to client code.
*
*  I chose to put most of the user interface into this record class,
*  so I do need the encapsulation used here.
*/
class TypeTableRecord
{
public:
	using Namespace = std::string;
	using Name = std::string;
	using Type = std::string;
	using FileName = std::string;
	using iterator = std::vector<Namespace>::iterator;

	Name& name() { return _name; }
	Type& type() { return _type; }
	FileName& fileName() { return _whereDefined; }
	void addNameSpace(const Namespace& ns)
	{
		if (find(_namespaces.begin(), _namespaces.end(), ns) == _namespaces.end())
			_namespaces.push_back(ns);

		
	}
	size_t numberOfNameSpaces() { return _namespaces.size(); }
	iterator begin() { return _namespaces.begin(); }
	iterator end() { return _namespaces.end(); }
	Namespace operator[](size_t n) const
	{
		if (n < 0 || n >= _namespaces.size())
			throw(std::exception("index out of range"));
		return _namespaces[n];
	}
private:
	Name _name;
	Type _type;
	FileName _whereDefined;
	std::vector<Namespace> _namespaces;
};

/////////////////////////////////////////////////////////////////////
// beginning of solution
//   - needs a few words about the record structure
//   - With a Record class that holds required information the
//     Table only needs to add records, store them, and provide access.

template<typename TableRecord>
class TypeTable
{
public:
	using iterator = typename std::vector<TableRecord>::iterator;

	void addRecord(const TableRecord& record)
	{
		_records.push_back(record);
	}
	TableRecord& operator[](size_t n)
	{
		if (n < 0 || n >= _records.size())
			throw(std::exception("index out of range"));
		return _records[n];
	}
	TableRecord operator[](size_t n) const
	{
		if (n < 0 || n >= _records.size())
			throw(std::exception("index out of range"));
		return _records[n];
	}
	iterator begin() { return _records.begin(); }
	iterator end() { return _records.end(); }
private:
	std::vector<TableRecord> _records;
};

// end of solution
/////////////////////////////////////////////////////////////////////

template<typename TableRecord>
void showRecord(TableRecord& record)
{
	std::cout << "\n  ";
	std::cout << std::setw(16) << record.fileName();
	std::cout << std::setw(8) << record.name();
	std::cout << std::setw(12) << record.type();
	for (auto ns : record)
		std::cout << std::setw(16) << ns;
}

void showRecordHeader()
{
	std::cout << std::left << "\n  ";
	std::cout << std::setw(16) << "Filename";
	std::cout << std::setw(8) << "Name";
	std::cout << std::setw(12) << "Type";
	std::cout << std::setw(16) << "Namespaces" << "\n";
	std::cout << " " << std::setw(16) << std::string(14, '-');
	std::cout << std::setw(8) << std::string(6, '-');
	std::cout << std::setw(12) << std::string(10, '-');
	std::cout << std::setw(16) << std::string(14, '-');
}

template<typename TableRecord>
void showTypeTable(TypeTable<TableRecord>& table)
{
	showRecordHeader();
	for (auto record : table)
		showRecord(record);
}

template<typename TableRecord>
bool TypeTable<TableRecord>::ParseTree(element* pItem, string file)
{
	using Namespace = std::vector<std::string>;
	using Name = std::string;
	using Type = std::string;
	using FileName = std::string;
	using iterator = std::vector<Namespace>::iterator;
	using FileMap = std::unordered_map<FileName, std::vector<Namespace>>;
	static Namespace nameSpaceVector;
	static vector<int> namespaceCountVector;
	TableRecord record;
	if (pItem != nullptr) {
		static int indentLevel = 0;
		static int prevIndentLevel = 0;
		if (indentLevel != 0)
			if (pItem->type == "namespace") {
				nameSpaceVector.push_back(pItem->name);
				namespaceCountVector.push_back(indentLevel);
			}
		for (std::vector<int>::reverse_iterator itr = namespaceCountVector.rbegin(); itr != namespaceCountVector.rend(); ++itr) {
			if (indentLevel < *itr)
				nameSpaceVector.pop_back();
		}
		record.name() = pItem->name;
		record.type() = pItem->type;
		record.getFileMap()[file].push_back(nameSpaceVector);
		this->addRecord(record);
		auto iter = pItem->_children.begin();
		++indentLevel;
		while (iter != pItem->_children.end())
		{
			return ParseTree(*iter, file);
			++iter;
		}
		--indentLevel;
	}
	else
		cout << "Null Element";
}



template<typename TableRecord>
bool TypeTable<TableRecord>::treetotable(AST<ASTelement*>* AStree)
{
	ASTelement* pItem = AStree->Root();
	if (pItem != nullptr)
	{
		ParseTree(pItem, AStree->addfilename(name));
	}

}

using namespace Utilities;
//using Utils = StringHelper;



void main()
{
	/*Utils::Title("MT4Q1 - TypeTable");
	putline();*/

	TypeTable<TypeTableRecord> table;

	TypeTableRecord record;
	/*record.name() = "X";
	record.type() = "class";
	record.fileName() = "X.h";
	record.addNameSpace("TypeAnalysis");
	record.addNameSpace("MT4");

	table.addRecord(record);

	record.name() = "fun";
	record.type() = "method";
	record.fileName() = "X.h";
	record.addNameSpace("TypeAnalysis");
	record.addNameSpace("MT4");

	table.addRecord(record);*/

	showTypeTable(table);
	std::cout << "\n\n";
}
