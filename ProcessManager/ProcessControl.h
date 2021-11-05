#pragma once
#include <Windows.h>
#include<list>
#include<tlhelp32.h>
#include<iostream>
class ProcessControl
{
private:
	HANDLE hProcess;
	std::list<DWORD> PCProcessTemp;
public:
	ProcessControl();
	std::list<DWORD> FindResult;
	std::list<DWORD> NextFindResult;
	void List();
	void OpenAndSelect(char* Path);
	void Select(long Index);
	void DeSelect();
	void Close();
	BOOL IsSelected();
	BOOL Find(DWORD Value);
	BOOL NextFind(DWORD Value);
	BOOL WriteALLMemoryToNextFinded(DWORD Value);
};

