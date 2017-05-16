#pragma once

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
- QWorkedITem.h, BlockingQ.h

Build commands:
==============
- devenv Project3DA.sln

Maintenance History:
====================
ver 1.0 : 7 Apr 16 - First Release

*/

///////////////////////////////////////////////////////////////////////
// QueuedWorkItems.h - child thread processes enqueued work items    //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016         //
///////////////////////////////////////////////////////////////////////
/*
* A single child thread processes work items equeued by main thread
*/

#include <thread>
#include <functional>
#include <mutex>
#include "BlockingQ.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items

template<typename Result>
class ProcessWorkItem
{
public:
	void start();
	void doWork(WorkItem<Result>* pWi);
	Result result();
	void wait();
	ProcessWorkItem(int n);
	~ProcessWorkItem();
private:
	std::vector<std::thread*> _pThread;
	BlockingQueue<WorkItem<Result>*> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
	std::mutex mtx;
	int num;
};

template<typename Result>
ProcessWorkItem<Result>::ProcessWorkItem(int n)
{
	num = n;
}

template<typename Result>
BlockingQueue<WorkItem<Result>*> getQ()
{
	return _workItemQueue;
}
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ProcessWorkItem<Result>::wait()
{
	for (auto thr : _pThread)
		thr->join();
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem<Result>* pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ProcessWorkItem<Result>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ProcessWorkItem<Result>::start()
{
	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			WorkItem<Result>* pWi = _workItemQueue.deQ();

			if (pWi == nullptr)
			{
				_workItemQueue.enQ(nullptr);
				return;
			}
			Result result = (*pWi)();
			_resultsQueue.enQ(result);
		}
	};
	for (int i = 0; i < num; i++) {
		_pThread.push_back(new std::thread(threadProc));
	}
}
//----< clean up heap >------------------------------------------------

template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
	for (auto thr : _pThread)
		delete thr;
}

