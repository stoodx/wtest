// wtestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../wtest/wtest.h"

using namespace stoodx;

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring strProc(L"firefox");
	if (!wtest::isProcessRunning(strProc))
	{
		_tprintf(_T("No process: %s\n"), strProc.c_str());
	}
	else
	{
		_tprintf(_T("Have the process: %s\n"), strProc.c_str());
		std::wstring strDLL(L"kernelbase.dll");
		if (wtest::isDllInProcess(strDLL, strProc))
		{
			_tprintf(_T("The process %s has %s\n"), strProc.c_str(), strDLL.c_str());
		}
		else
		{
			_tprintf(_T("The process %s doesn't have %s\n"), strProc.c_str(), strDLL.c_str());
		}
	}

	std::system("PAUSE");
	return 0;
}

