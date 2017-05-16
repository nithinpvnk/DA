#include "PDepAnalysis.h"

/////////////////////////////////////////////////////////////////////
//  PDepAnalysis.h - runs dependency analysis asynchronously	   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., SyracuseUniversity	   //
//                 vputchak@syr.edu								   //
/////////////////////////////////////////////////////////////////////

/*
Build Process:
==============
Required files
- ThreadPool.h, Tokenizer.h, Tokenizer.cpp, TypeTable.h
- FileMgr.h, DependencyExec.h, DependencyExec.cpp,PDepAnalysis.h

Build commands:
===============
- devenv project3DA.sln

Maintenance History:
====================
ver 1.0 : 07 Apr 16 - First release

*/


std::string PDepAnalysis::trimPath(std::string fullpath)
{
	const size_t last_slash_idx = fullpath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		fullpath.erase(0, last_slash_idx + 1);
	}
	return fullpath;
}



void PDepAnalysis::beginwork1() 
{
	 ProcessWorkItem<std::string> _ThreadPool(4);
	_ThreadPool.start();
	WorkItem<std::string> fileSearch = [&]()
	{
		std::cout << "thread ID : " << std::this_thread::get_id() << "\n" << "File Manager" << "\n";
		FileMgr fm(_path, _blockQ);
		for (auto pat : _patterns)
			fm.addPattern(pat);
		fm.search();
		end();
		return "";
	};
	std::string filepath;
	WorkItem<std::string> doDependency = [&]()
	{
		std::vector<std::string> fileSpec;
		std::cout << "Dependencies :" << "\n";
		std::cout << "================" << "\n";
		Toker tok;
		std::fstream in(filepath);
		tok.attach(&in);
		DepAnalysis dep(&tok, &MergedTypeTable, trimPath(filepath));
		dep.dependency();
		dep.showDep();
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
	
				//_ThreadPool.doWork(&doDependency);
				_ThreadPool.doWork(nullptr);
				std::this_thread::sleep_for(std::chrono::milliseconds(400));
				break;
			}
			else
			{
				filepath = top;
				_ThreadPool.doWork(&doDependency);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}
	_ThreadPool.wait();

}

#ifdef TEST_PDA
void main()
{
	TypeTable TypTbl;
	ProcessWorkItem<std::string> Threadpool(4);

	Threadpool.start();

	std::string path = "C:\Users\Nithin\Downloads\Parser-AST_603645986\nithin\Project2_AST";
	std::vector<std::string> pattrns;

	pattrns.push_back("*.h");
	pattrns.push_back("*.cpp");

	ParallelDA PDA(&Threadpool, &TypTbl);
	PDA.getLamda(&path, pattrns);
}
#endif