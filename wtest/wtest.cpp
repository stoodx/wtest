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
	
	std::vector<unsigned> vecPIDList;
	bRes = pWtest->FindProcessByName(strProcessName, vecPIDList);
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

	std::vector<unsigned> vecPIDList;
	if (!pWtest->FindProcessByName(strProcessName, vecPIDList))
	{//no such process
		delete pWtest;
		SetLastError(ERROR_INVALID_DATA);
		return bRes;
	}
	std::vector<std::wstring> vecListDll;
	unsigned nModules = 0;
	int nSize = vecPIDList.size();
	for (int i = 0; i < nSize; i++)
	{
		unsigned nPID = vecPIDList.at(i);
		nModules = pWtest->FindAllModulesOfProcess(nPID, &vecListDll);
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
			if (bRes)
				break;
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

bool wtest::getProcessIdByName(const std::wstring& strName,  std::vector<unsigned>& vecPIDList)
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

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		// to low case
		std::wstring strFilename(L"");
		std::wstring str = processInfo.szExeFile;
		for(auto elem : str)
			strFilename +=  std::tolower(elem, loc);
		if ( strLowName.compare(strFilename) == 0 )
		{
			vecPIDList.push_back(processInfo.th32ProcessID);
		}
	}
	CloseHandle(processesSnapshot);
	if (vecPIDList.empty())
		return false;
	return true;
}

bool wtest::FindProcessByName(const std::wstring& strName, std::vector<unsigned>& vecPIDList)
{
	std::wstring strLow(strName);
	 //try once again
	if (strLow.find(L".exe") == std::wstring::npos)
		strLow.append(L".exe");
	if (getProcessIdByName(strLow, vecPIDList))
		return true;
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

bool wtest::startProcess(const wchar_t* strProcessName,  bool bWaitForFinish, const wchar_t* strParameters)
{
	if (!strProcessName)
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
	if (bWaitForFinish)
		bRes = pWtest->StartProcessAndWaitForFinish(strProcessName);
	else
		bRes = pWtest->StartProcessAndContinue(strProcessName, strParameters);
	delete pWtest;
	return bRes;
}

bool wtest::startProcessAsAdminAndWaitForFinish(const wchar_t* strProcessName, const wchar_t* strPassword)
{
	if (!strProcessName || !strPassword)
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	STARTUPINFOW su_info;
	ZeroMemory(&su_info, sizeof(STARTUPINFOW));
	su_info.cb = sizeof(STARTUPINFOW);

	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));

	BOOL bResult =  CreateProcessWithLogonW(	L"Administrator", 
												L"WIN8", //L"localhost",
												strPassword,
												LOGON_WITH_PROFILE,//0,
												strProcessName,
												NULL,
												0, //CREATE_NEW_CONSOLE,
												NULL,
												NULL,
												&su_info,
												&processInformation);

   if (!bResult)
 	   return false;

    // Successfully created the process.  Wait for it to finish.
    WaitForSingleObject( processInformation.hProcess, INFINITE );
 
    // Get the exit code.
	DWORD  dwExitCode = 0;
    bResult = GetExitCodeProcess(processInformation.hProcess, &dwExitCode);

    // Close the handles.
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );

    if (!bResult)
	   return false; 
	return true;
}

bool wtest::StartProcessAndWaitForFinish(const wchar_t* strProcessName)
{
	if (!strProcessName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}
   PROCESS_INFORMATION processInformation = {0};
   STARTUPINFO startupInfo                = {0};
   startupInfo.cb                         = sizeof(startupInfo);

   // Create the process
   BOOL bResult = CreateProcess(NULL, (LPWSTR)strProcessName, 
                               NULL, NULL, FALSE, 
                               NORMAL_PRIORITY_CLASS | DETACHED_PROCESS, 
                               NULL, NULL, &startupInfo, &processInformation);
   if (!bResult)
	   return false;

    // Successfully created the process.  Wait for it to finish.
    WaitForSingleObject( processInformation.hProcess, INFINITE );
 
    // Get the exit code.
	DWORD  dwExitCode = 0;
    bResult = GetExitCodeProcess(processInformation.hProcess, &dwExitCode);

    // Close the handles.
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );

    if (!bResult)
	   return false; 
	return true;
}

bool wtest::StartProcessAndContinue(const wchar_t* strProcessName, const wchar_t* strParameters)
{
	if (!strProcessName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return false;
	}

	int nRes = (int)ShellExecute(NULL, NULL, L"chrome.exe", strParameters, NULL, SW_SHOWNORMAL);   //new-window-tab 

	if (nRes < 32)
		return false;
	else
		return true;
}

bool wtest::isRunAsAdmin()
{
    bool bRet = false;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess(),TOKEN_QUERY,&hToken ) ) 
	{
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) 
		{
            if (Elevation.TokenIsElevated)
				bRet = true;
        }
    }
    if( hToken ) 
	{
        CloseHandle( hToken );
    }
    return bRet;
}

__int64 wtest::getFileSize(const wchar_t* strFileName)
{
	if (!strFileName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return -1;
	}
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 
								FILE_SHARE_READ, NULL, OPEN_EXISTING, 
								FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
		return -1; 
	LARGE_INTEGER liFileSize;
    if (!GetFileSizeEx(hFile, &liFileSize))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return liFileSize.QuadPart;
}
