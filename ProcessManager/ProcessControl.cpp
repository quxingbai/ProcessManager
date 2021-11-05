#include "ProcessControl.h"
#define LError() cout<<"����쳣["<<GetLastError()<<"]"<<endl;

ProcessControl::ProcessControl() {

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		std::cout << "��ȡ�������̿���ʧ��..." << std::endl;
		return;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL CanNext = Process32First(snap, &pe);
	int Index = 1;
	while (CanNext) {
		CanNext = Process32Next(snap, &pe);
		Index += 1;
		this->PCProcessTemp.push_back(pe.th32ProcessID);
	}
	CloseHandle(snap);
}

void ProcessControl::List() {
	this->PCProcessTemp.clear();
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		std::cout << "��ȡ�������̿���ʧ��..." << std::endl;
		return;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL CanNext = Process32First(snap, &pe);
	int Index = 1;
	while (CanNext) {
		printf(" %u\n", Index);
		printf(" �������ƣ�%ls\n", pe.szExeFile);
		printf(" ����ID��%u\n\n", pe.th32ProcessID);
		CanNext = Process32Next(snap, &pe);
		Index += 1;
		this->PCProcessTemp.push_back(pe.th32ProcessID);
	}
	CloseHandle(snap);
}
void  ProcessControl::OpenAndSelect(char* Path) {

}
void  ProcessControl::Select(long Index) {
	Index -= 1;
	long id = 0;
	for (auto f = this->PCProcessTemp.begin(); f != this->PCProcessTemp.end(); f++) {
		id += 1;
		if (id == Index) {
			this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, *f);
			if (IsSelected()) {
				std::cout << "�ɹ�ѡ�У�" << *f << std::endl;
			}
			else {
				std::cout << "ѡ��ʧ��" << std::endl;
			}
			return;
		}
	}
	std::cout << "ѡ��ʧ��\n";
}
void  ProcessControl::DeSelect() {
	this->hProcess = NULL;
}
void  ProcessControl::Close() {
	TerminateProcess(this->hProcess, 0);
	CloseHandle(this->hProcess);
}
BOOL  ProcessControl::IsSelected() {
	return this->hProcess != NULL;
}
BOOL  ProcessControl::Find(DWORD Value) {
	if (!IsSelected()) {
		std::cout << "δѡ���κν���" << std::endl;
		return FALSE;
	}
	this->FindResult.clear();
	this->NextFindResult.clear();
	MEMORY_BASIC_INFORMATION mbi;
	for (DWORD f = 0x0400000; f < 0x3FFFFFFF; f += mbi.RegionSize) {
		VirtualQueryEx(this->hProcess, (LPCVOID)f, &mbi, sizeof(mbi));
		if (mbi.State != MEM_COMMIT) {
			continue;
		}
		BYTE* readTemp = new BYTE[mbi.RegionSize];
		LPCVOID point = (LPCVOID)f;
		BOOL CanRead = ReadProcessMemory(this->hProcess, point, readTemp, mbi.RegionSize, NULL);
		if (CanRead) {
			for (int i = 0; i < mbi.RegionSize; i++) {
				DWORD* dw = (DWORD*)&readTemp[i];
				if (dw[0] == Value) {
					this->FindResult.push_back((DWORD)point + i);
					this->NextFindResult.push_back((DWORD)point + i);
				}
			}
		}
	}
	std::cout << "�ҵ���" << FindResult.size() << "��" << std::endl;
	return TRUE;
}
BOOL  ProcessControl::NextFind(DWORD Value) {
	std::list<DWORD> l;
	for (auto f = NextFindResult.begin(); f != NextFindResult.end(); f++) {
		DWORD dw = NULL;
		BOOL canRead = ReadProcessMemory(this->hProcess, (LPCVOID)*f, &dw, sizeof(DWORD), NULL);
		if (!canRead) {
			std::cout << "��ȡʧ��" << GetLastError() << std::endl;
		}
		if (dw == Value) {
			l.push_back(*f);
		}
	}
	this->NextFindResult.clear();
	this->NextFindResult = l;
	std::cout << "�ҵ���" << NextFindResult.size() << "��" << std::endl;
	return TRUE;
}

BOOL ProcessControl::WriteALLMemoryToNextFinded(DWORD Value) {
	for (auto f = NextFindResult.begin(); f != NextFindResult.end(); f++) {
		BOOL canWrite = WriteProcessMemory(this->hProcess, (LPVOID)*f, &Value, sizeof(DWORD), NULL);
		if (!canWrite) {
			std::cout << "д��ʧ��" << GetLastError() << std::endl;
		}
		else {
			std::cout << "�ɹ���" << (LPVOID)*f << "д��Ϊ" << Value << std::endl;
		}
	}
	return TRUE;
}