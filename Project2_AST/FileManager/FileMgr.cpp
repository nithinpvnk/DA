/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns            //
//               on a specified path                               //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      MacBook Pro, Windows 7 Professional	           //
//  Application:   Project for CSE687 Pr3, Sp16	                   //
//  Author:		   Venkata Nithin Kumar P., Syracuse University	   //
//                 vputchak@syr.edu								   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

/*
Build Process :
== == == == == == ==
Required files
- FileMgr.h,FileSystem.h

Build commands :
== == == == == == ==
-devenv Project3DA.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 7 Apr 16 - First Release

*/

#include "FileMgr.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <iostream>


void FileMgr::addPattern(const std::string & patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
	patterns_.push_back(patt);
}

void FileMgr::search()
{
	find(path_);
}

void FileMgr::find(const std::string & path)
{

	std::string fpath = FileSystem::Path::getFullFileSpec(path);

	for (auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
		for (auto f : files)
		{
			vFiles.push_back(fpath + "\\" + f);
			blockQ_->enQ(fpath + "\\" + f);
		}
	}
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
	for (auto d : dirs)
	{
		if (d == "." || d == "..")
			continue;
		std::string dpath = fpath + "\\" + d;  // here's the fix
		find(dpath);
	}
}

std::vector<std::string> FileMgr::getFiles()
{
	return vFiles;
}


#ifdef TEST_FILEMGR

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing FileMgr";
	std::cout << "\n =================";

	DataStore ds;
	FileMgr fm(argv[1], ds);
	for (int i = 2; i < argc; ++i)
		fm.addPattern(argv[i]);
	//fm.addPattern("*.h");
	//fm.addPattern("*.cpp");
	//fm.addPattern("*.partial");
	fm.search();
	fm.getFiles();

	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n ---------------------------";
	for (auto fs : ds)
	{
		std::cout << "\n  " << fs;
	}
	std::cout << "\n\n";
	return 0;
}
#endif


