#include "Task.h"
///////////////////////////////////////////////////////////////////////
// Task.cpp-provides callable obj to run Type analysis asynchronously//
//  ver 1.0                                                          //
//  Language:      Visual C++ 2015, SP1                              //
//  Platform:      MacBook Pro, Windows 7 Professional	             //
//  Application:   Project for CSE687 Pr3, Sp16	                     //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	     //
//                 vputchak@syr.edu								     //
///////////////////////////////////////////////////////////////////////

/*
Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First release

*/

Task::Task(std::string path, std::vector<std::string> patterns)
{
	_patterns = patterns;
	_path = path;
	_blockQ = new BlockingQueue<std::string>();
	beginwork();
}

void Task::beginwork()
{
	std::string file;
   ProcessWorkItem<std::string> _ThreadPool(4);
	_ThreadPool.start();

	WorkItem<std::string> fileSearch = [&]()
	{
	
		filepatternsearch();
		return "";
	};

	WorkItem<std::string> parseFile = [&]()
	{
		parserfile(file);
		return "";
	};

	WorkItem<std::string> mergeTables = [&]()
	{
		mergertable();
		return "";
	};

	_ThreadPool.doWork(&fileSearch);
	while (true)
	{
		if (_blockQ != NULL && _blockQ->size() > 0)
		{
			std::string top = _blockQ->deQ();
			if (top.compare("Completion of the file") == 0)
			{
				_ThreadPool.doWork(&mergeTables);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				PDepAnalysis pda(MergedTypeTable, _path, _patterns, _ThreadPool);
				pda.beginwork1();
				break;
			}
			else
			{
				file = top;
				_ThreadPool.doWork(&parseFile);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	}
	_ThreadPool.wait();
}

std::string Task::trimPath(std::string fullpath)
{
	const size_t last_slash_idx = fullpath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		fullpath.erase(0, last_slash_idx + 1);
	}
	return fullpath;
}

void Task::filepatternsearch()
{
	std::cout << "thread ID : " << std::this_thread::get_id() << "\t" << "File Manager" << "\n";
	FileMgr fm(_path, _blockQ);
	for (auto pat : _patterns)
		fm.addPattern(pat);
	fm.search();
	end();
}

void Task::parserfile(std::string file)
{
	ConfigParseToConsole configure(trimPath(file));
	Parser* pParser = configure.Build();

	if (pParser)
	{
		if (!configure.Attach(file))
			std::cout << "\n  could not open file " << file << std::endl;
	}
	else
		std::cout << "\n\n  Parser not built\n\n";

	// now that parser is built, use it

	while (pParser->next())
		pParser->parse();

	TypeTables.push_back(configure.getTypeTable());
	std::cout << "thread ID : " << std::this_thread::get_id() << "\t" << trimPath(file) << "\n";
}

void Task::mergertable()
{
	for (auto tbl : TypeTables)
		MergedTypeTable = MergedTypeTable.Merge(MergedTypeTable, *tbl);

	std::cout << "\n" << "Merged Type Table" << "\t" << "thread ID : " << std::this_thread::get_id() << "\n";
	std::cout << "====================";
	MergedTypeTable.showTypeTable(MergedTypeTable);
	std::cout << "\n \n";

}

#ifdef TEST_TASK

void main()
{
	std::string path = "C:\Users\Nithin\Downloads\Parser-AST_603645986\nithin\Project2_AST";
	std::vector<std::string> pattrns;

	pattrns.push_back("*.h");
	pattrns.push_back("*.cpp");

	Task tsk(path, pattrns);
}

#endif