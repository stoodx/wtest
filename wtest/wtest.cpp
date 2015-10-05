#include <Windows.h>
#include <TlHelp32.h>
#include <locale> 

#include "wtest.h"

using namespace stoodx;

bool wtest::isProcessRunning(const std::wstring& strProcessName)
{
	if (strProcessName.empty())
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}

	bool bRes = false;
	wtest* pWtest = NULL;
	pWtest = new wtest;
	if (pWtest == NULL)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return bRes;
	}
	
	bRes = pWtest->FindProcessByName(strProcessName);
	delete pWtest;
	SetLastError(ERROR_SUCCESS);
	return bRes;
}

bool wtest::isDllInProcess(const std::wstring& strDllName,
					const std::wstring& strProcessName)
{
	if (strProcessName.empty() || strDllName.empty())
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	wtest* pWtest = NULL;
	pWtest = new wtest;
	if (pWtest == NULL)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}

	delete pWtest;
	SetLastError(ERROR_SUCCESS);
	return true;
}

int wtest::GetProcessIDByName(const std::wstring& strName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;
	Process32First(processesSnapshot, &processInfo);

	int nCount = 0;
	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( strName.compare(processInfo.szExeFile) == 0 )
		{
			nCount++;
		}
	}
	CloseHandle(processesSnapshot);
	if (nCount > 0)
		return processInfo.th32ProcessID;
	else
		return 0;
}

bool wtest::FindProcessByName(const std::wstring& strName)
{
	if (GetProcessIDByName(strName))
		return true;

	//try to low case
	std::wstring strLow(L"");
	std::locale loc;
	 for(auto elem : strName)
		 strLow +=  std::tolower(elem, loc);
	 if (GetProcessIDByName(strLow))
		 return true;
	 //try once again
	 strLow.append(L".exe");
	 if (GetProcessIDByName(strLow))
		 return true;

	 //try to upper case
	std::wstring strUp(L"");
	 for(auto elem : strName)
		 strUp +=  std::toupper(elem, loc);
	 if (GetProcessIDByName(strUp))
		 return true;
	 //try once again
	 strUp.append(L".EXE");
	 if (GetProcessIDByName(strUp))
		 return true;
	 else
		return false;
}