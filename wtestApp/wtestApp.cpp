// wtestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../wtest/wtest.h"

using namespace stoodx;

int _tmain(int argc, _TCHAR* argv[])
{
	std:: wstring strProc(L"firefox");
	if (!wtest::isProcessRunning(strProc))
	{
		_tprintf(_T("No process: %s\n"), strProc.c_str());
	}
	else
	{
		_tprintf(_T("Have the process: %s\n"), strProc.c_str());
	}

	std::system("PAUSE");
	return 0;
}

