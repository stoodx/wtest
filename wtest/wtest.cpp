#include <Windows.h>
#include <TlHelp32.h>
#include <locale> 
#include <Psapi.h>

//#ifdef _DEBUG
//#include <iostream>
//#endif // DEBUG

#include "wtest.h"
#include "TaskScheduler.h"

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
	std::vector<std::wstring> vecListDll;
	unsigned nModules = 0;
	nModules = pWtest->FindAllModulesOfProcess(dwPID, &vecListDll);
	if (nModules)
	{
		//dll name to low case 
		std::locale loc;
		std::wstring strDllLowName(L"");
		for(auto elem :  strDllName)
			 strDllLowName +=  std::tolower(elem, loc);
		if (strDllLowName.find(L".dll") == std::wstring::npos)
			strDllLowName.append(L".dll");
		for(unsigned i = 0; i < nModules; i++)
		{
			if (vecListDll[i].find(strDllLowName) != std::wstring::npos)
			{
				bRes = true;
				break;
			}
		}
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
    if (NULL == hProcess)
        return 0;

   // Get a list of all the modules in this process.
    if( EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
    {
		std::locale loc;
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
			TCHAR strModName[MAX_PATH] = {0};

            // Get the full path to the module's file.
            if ( GetModuleFileNameEx( hProcess, hMods[i], strModName,
                                      sizeof(strModName) / sizeof(TCHAR)))
            {
				//to low case
				std::wstring strLowName(L"");
				for(auto elem : strModName)
					strLowName +=  std::tolower(elem, loc);
				pvecListModules->push_back(strLowName);
            }
        }
    }
    
    // Release the handle to the process.
    CloseHandle( hProcess );

	return pvecListModules->empty() ? 0 : pvecListModules->size();
}

bool wtest::isProcessRunning(unsigned dwProcId)
{
	bool bRes = false;
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;
	Process32First(processesSnapshot, &processInfo);

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( processInfo.th32ProcessID ==  dwProcId)
		{
			bRes = true;
			break;
		}
	}
	CloseHandle(processesSnapshot);

	SetLastError(ERROR_SUCCESS);
	return bRes;
}

bool wtest::doesTaskExists(const std::wstring& strTaskName)
{
	TaskScheduler ts;
	return ts.doesTaskExists(strTaskName);
}

unsigned wtest::getProcessIdByName(const std::wstring& strName)
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
	std::wstring strLow(strName);
	dwPID =  getProcessIdByName(strLow);
	if (dwPID)
		return true;
	 //try once again
	strLow.append(L".exe");
	dwPID =  getProcessIdByName(strLow);
	if (dwPID)
		return true;
	else
		return false;
}

bool wtest::isFileExist(const wchar_t* strName)
{
	if (!strName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	return ::GetFileAttributes(strName) != DWORD(-1);
}

bool wtest::closeProcess(std::wstring strName)
{
	bool bRes = false;
	if (strName.empty())
	{
		SetLastError(ERROR_INVALID_DATA);
		return bRes;
	}

	//try to low case
	std::locale loc;
	std::wstring strLowName(L"");
	for(auto elem : strName)
		strLowName +=  std::tolower(elem, loc);
	if (strLowName.find(L".exe") == std::wstring::npos)
		strLowName.append(L".exe");

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
			if ( strLowName.compare(entry.szExeFile) == 0)
            {  
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				if (TerminateProcess(hProcess, 0))
					bRes = true;
                CloseHandle(hProcess);
				break;
            }
        }
    }

    CloseHandle(snapshot);

	return bRes;
}


bool wtest::isDirectoryExist(const wchar_t* strName)
{
	if (!strName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	SHFILEINFO shFileInfo;
	if (SHGetFileInfo(strName, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}