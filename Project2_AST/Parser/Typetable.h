#ifndef TYPETABLE
#define TYPETABLE

///////////////////////////////////////////////////////////////////////
// MT4Q1.cpp - TypeTable                                             //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Midterm Spring 2016 //
///////////////////////////////////////////////////////////////////////

/*

Module Operations:
==================
* Record is a map containing name, namespace, type and filename
* Table is a set of records. Provide methods to add records to the table
* and print them to the console.

Public Interface:
=================
* Record()		- creates a new type table record
* showRecord()	- prints a record to the console
* recordMerge()	- merges two records to one
* addRecord()	- adds a record to the type table
* get()			- returns type table records
* merge()		- merges the type table
* showTypeTable()- prints the table to the console

Build commands :
== == == == == == ==
-devenv Project3DA.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 4 Apr 16 - First Release
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../Utilities/Utilities.h"
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
	using Parent = std::string;
	using FileName = std::string;
	using iterator = std::vector<Namespace>::iterator;

	Name& name() { return _name; }
	Type& type() { return _type; }
	Parent& parent() { return _parent; }
	FileName& fileName() { return _whereDefined; }
	void addNameSpace(const Namespace& ns)
	{
		if (find(_namespaces.begin(), _namespaces.end(), ns) == _namespaces.end())
			_namespaces.push_back(ns);
		//mapNamespace.insert(_whereDefined, ns);
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
	Parent _parent;
	std::vector<Namespace> _namespaces;
	//std::unordered_map<FileName, std::vector<Namespace>> mapNamespace;
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
	TypeTable<TableRecord> Merge(TypeTable<TableRecord> table1, TypeTable<TableRecord> table2)
	{
		for (auto rec : table2._records)
			table1.addRecord(rec);
		return table1;
	}

	void showTypeTable(TypeTable<TableRecord>& table)
	{
		showRecordHeader();
		for (auto record : table)
			showRecord(record);
	}
private:
	std::vector<TableRecord> _records;
	void showRecordHeader()
	{
		std::cout << std::left << "\n  ";
		std::cout << std::setw(20) << "Filename";
		std::cout << std::setw(25) << "Name";
		std::cout << std::setw(16) << "Type";
		//std::cout << std::setw(20) << "Parent";
		std::cout << std::setw(16) << "Namespaces" << "\n";
		std::cout << " " << std::setw(20) << std::string(14, '-');
		std::cout << std::setw(25) << std::string(15, '-');
		std::cout << std::setw(16) << std::string(10, '-');
		//std::cout << std::setw(20) << std::string(16, '-');
		std::cout << std::setw(16) << std::string(14, '-');
	}
	void showRecord(TableRecord& record)
	{
		std::cout << "\n  ";
		std::cout << std::setw(20) << record.fileName();
		std::cout << std::setw(25) << record.name();
		std::cout << std::setw(16) << record.type();
		//std::cout << std::setw(20) << record.parent();
		for (auto ns : record)
			std::cout << std::setw(16) << ns;
	}
};

// end of solution
/////////////////////////////////////////////////////////////////////

#endif // !TYPETBL

