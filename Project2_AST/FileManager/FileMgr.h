#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns on a         //
//             specified path						               //
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
*
* Package Operations :
* ====================
*FileMgr uses the services of FileSystem to find files and stores its findings
* efficiently in DataStore.
* Finds all files, matching a set of patterns, along with their paths.
* Stores each filename and path only once, using DataStore.
*
*
* Public interface:
* ===============
* FileManager(const std::string& path, FilePatterns& patternVector, DataStore& ds) : path_(path), patterns(patternVector), dataStore_(ds){}
* -- This constructor takes path as input and it will be use like that from where files needs to be analyze,      datastore refernce to store
* -- Results will be store in datastore refernce.
* void search() - Searches the folder
* void find(const std::string &path, bool isSubDir);
* -- find files as like provided path.
*
* Build commands:
* ==============
* - devenv Project3DA.sln
*
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp
*
*
* ver 1.0 : 7 Apr 16 - First Release
*/

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../ThreadPool/BlockingQ.h"
#include <iostream>

class FileMgr
{
public:

	FileMgr(const std::string& path, BlockingQueue<std::string>* bQ) 
	{
		blockQ_ = bQ;
		path_ = path;
		patterns_.push_back("*.*");
	};

	void addPattern(const std::string& patt);
	void search();
	void find(const std::string& path);
	std::vector<std::string> getFiles();

private:
	BlockingQueue<std::string>* blockQ_;
	using patterns = std::vector<std::string>;
	std::vector<std::string> vFiles;
	std::string path_;
	patterns patterns_;
	
};

#endif

