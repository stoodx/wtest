#include <Windows.h>
#include <TlHelp32.h>
#include <locale> 
#include <Psapi.h>

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
	
	unsigned dwPID = 0;
	bRes = pWtest->FindProcessByName(strProcessName, dwPID);
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

	bool bRes = false;
	wtest* pWtest = NULL;
	pWtest = new wtest;
	if (pWtest == NULL)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return bRes;
	}

	unsigned dwPID = 0;
	if (!pWtest->FindProcessByName(strProcessName, dwPID))
	{//no such process
		delete pWtest;
		SetLastError(ERROR_INVALID_DATA);
		return bRes;
	}
	std::vector<std::wstring> vecListDLL;
	unsigned nModules = 0;
	nModules = pWtest->FindAllModulesOfProcess(dwPID, &vecListDLL);
	if (nModules)
	{
		bRes = true;
	}

	delete pWtest;
	SetLastError(ERROR_SUCCESS);
	return bRes;
}

int wtest::FindAllModulesOfProcess(unsigned dwPID, std::vector<std::wstring>* pvecListModules)
{
	//return size number
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned i;

    // Get a handle to the process.
    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
							FALSE, dwPID );
	Sleep(1000);
    if (NULL == hProcess)
        return 0;

   // Get a list of all the modules in this process.
    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
			TCHAR strModName[MAX_PATH] = {0};

            // Get the full path to the module's file.
            if ( GetModuleFileNameEx( hProcess, hMods[i], strModName,
                                      sizeof(strModName) / sizeof(TCHAR)))
            {
				std::wstring str(strModName);
				pvecListModules->push_back(str);
            }
        }
    }
    
    // Release the handle to the process.
    CloseHandle( hProcess );

	return pvecListModules->empty() ? 0 : pvecListModules->size();
}

unsigned wtest::GetProcessIDByName(const std::wstring& strName)
{
	//compare only in low case 
	std::locale loc;
	std::wstring strLowName(L"");
	for(auto elem : strName)
		strLowName +=  std::tolower(elem, loc);

	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;
	Process32First(processesSnapshot, &processInfo);

	int nCount = 0;
	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		// to low case
		std::wstring strFilename(L"");
		std::wstring str = processInfo.szExeFile;
		for(auto elem : str)
			strFilename +=  std::tolower(elem, loc);
		if ( strLowName.compare(strFilename) == 0 )
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

bool wtest::FindProcessByName(const std::wstring& strName,  unsigned& dwPID)
{
	//try to low case
	std::wstring strLow(strName);
	dwPID =  GetProcessIDByName(strLow);
	if (dwPID)
		return true;
	 //try once again
	strLow.append(L".exe");
	dwPID =  GetProcessIDByName(strLow);
	if (dwPID)
		return true;
	else
		return false;
}