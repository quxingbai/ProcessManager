// ProcessManager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include "ProcessControl.h"

using namespace std;
void Menu() {
	cout << "1：列表" << endl;
	cout << "2：选择" << endl;
	cout << "3：重新查找" << endl;
	cout << "4：连续查找" << endl;
	cout << "5：写入内存至连续查找内容" << endl;
	cout.clear();
}
int main()
{
	Menu();
	ProcessControl pc;
	while (true) {
		char code;
		cin >> code;
		switch (code)
		{
		case '1':
			pc.List();
			; break;
		case '2':
			long id;
			cout << "输入进程位数" << endl;
			cin >> id;
			pc.Select(id);
			; break;
		case '3': {
			DWORD val;
			cout << "输入整数值" << endl;
			cin >> val;
			pc.Find(val);
		}; break;
		case'4': {

			DWORD val;
			cout << "输入整数值" << endl;
			cin >> val;
			pc.NextFind(val);
		}; break;
		case'5': {
			DWORD val;
			cout << "输入整数值" << endl;
			cin >> val;
			pc.WriteALLMemoryToNextFinded(val);
		}
			   ; break;
		default:
			break;
		}
		Menu();
	}
}