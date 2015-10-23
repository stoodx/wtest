#include "stdafx.h"
#include <iostream>
#include <ShlObj.h>

#include "../wtest/wtest.h"
#include "DTTester.h"

using namespace stoodx;
#define RETURN_IF_FALSE(x) if(!x) return false

#define RUN_MSG std::cout << "[ RUN      ] " << __FUNCTION__ << std::endl;
#define RUN_MSG_NOT_IMPLEMENTED std::cout << "[ RUN      ] " << __FUNCTION__ << std::endl;\
	std::cout << "[ NOT IMPL ] " << __FUNCTION__ << std::endl;

#define DT_ASSERT(x) if(x == false) \
						{\
							std::cout << "[    FAIL ] " << std::endl;\
							m_nReturnCode = 1;\
							return;\
						}\
						else\
							std::cout << "[      OK  ] " << std::endl;\

DTTester::DTTester(const std::wstring& strInstallerPath,
				   const std::wstring& strUninstallerPath,
				   bool bStepByStep)
	: m_nReturnCode(0)
	, m_bPause(bStepByStep)
	, m_strInstallerPath(strInstallerPath)
	, m_strUninstallerPath(strUninstallerPath)
{
	//1. Check path to kit and install
	DT_ASSERT(checkParams());

	//2. Install the tracker
	DT_ASSERT(installTracker(m_strInstallerPath));

	//3. Check if tracker is installed
	DT_ASSERT(isTrackerInstalled());
/*
	//4. Check if tracker is running
	DT_ASSERT(isTrackerRunning());

	//5. Check if tracker task exists
	DT_ASSERT(doesTrackerTaskExist());

	//6. Check if config was fetched
	DT_ASSERT(isConfigFetched()); //not released

	//7. Check that config is not empty
	DT_ASSERT(isConfigNotEmpty()); //not released

	//8. Check that snss_analyzer is fetched
	DT_ASSERT(isSnssAnalyserFetched());

	//9. Check that snss_analyzer is not empty
	DT_ASSERT(isSnssAnalyserNotEmpty());

	//10. Run chrome ( with prev tabs )
	DT_ASSERT(runChrome());

	//11.is Chrome Running;
	DT_ASSERT(isChromeRunning());

	//12. Check if chrome has opener.dll
	DT_ASSERT(isOpenerDllInjectedInChrome());

	//13. Close chrome
	DT_ASSERT(WAIT(5000));
	DT_ASSERT(closeChrome());

	//14. Check if folder "temp" was created
	DT_ASSERT(isTempCreated());

	DT_ASSERT(FINISH_TEST());*/
}


DTTester::DTTester(void)
{
}

DTTester::~DTTester(void)
{
}

bool DTTester::installTracker(const std::wstring& strInstallerPath)
{
	RUN_MSG;
	m_strInstallerPath = strInstallerPath;

	RETURN_IF_FALSE(closeChrome());
	RETURN_IF_FALSE(wtest::startProcessAsAdminAndWaitForFinish(strInstallerPath.c_str(), 2500));

	return true;
}

bool DTTester::isTrackerInstalled()
{
	RUN_MSG;
	std::wstring strInstDir = getInstDir();
	RETURN_IF_FALSE(wtest::doesDirectoryExist(strInstDir.c_str()));

	std::wstring strOpenerDllPath(strInstDir);
	std::wstring strTrackerExePath(strInstDir);
	strOpenerDllPath.append(L"\\Opener.dll");
	strTrackerExePath.append(L"\\System health kit.exe");
	RETURN_IF_FALSE(wtest::doesFileExist(strOpenerDllPath.c_str()));
	RETURN_IF_FALSE(wtest::doesFileExist(strTrackerExePath.c_str()));

	return true;
}

bool DTTester::checkParams(void)
{
	RUN_MSG;
	if (!wtest::doesFileExist(m_strInstallerPath.c_str()) ||
		!wtest::doesFileExist(m_strUninstallerPath.c_str()))
	{
		std::wcout << L"error: "<< GetLastError() << std::endl;
		std::wcout << L"installer path: "<< m_strInstallerPath << std::endl;
		std::wcout << L"uninstaller path: "<< m_strUninstallerPath << std::endl;
		m_nReturnCode = 1;	
		return false;
	}
	return true;
}

bool DTTester::isTrackerRunning()
{
	RUN_MSG;
	std::wstring strTrackerName(L"System health kit.exe");
	return wtest::isProcessRunning(strTrackerName);
}

bool DTTester::doesTrackerTaskExist()
{
	RUN_MSG;
	std::wstring strTaskName(L"System Health Kit");
	return wtest::doesTaskExists(strTaskName);
}

bool DTTester::isConfigFetched()
{
	RUN_MSG_NOT_IMPLEMENTED;
	return false;
}

bool DTTester::isConfigNotEmpty()
{
	RUN_MSG_NOT_IMPLEMENTED;
	return false;
}

bool DTTester::isSnssAnalyserFetched()
{
	RUN_MSG
	//define directory of kit
	wchar_t* strKitName = L"System health kit";
	wchar_t strP[_MAX_PATH * sizeof(TCHAR)] = {0};
	if (!SHGetSpecialFolderPath(NULL, strP, CSIDL_LOCAL_APPDATA, TRUE))
	{
		m_nReturnCode = 1;
		return false;
	}
	std::wstring strFullPath = getInstDir();
	strFullPath.append(L"\\snss_analyser.js");	
	return wtest::doesFileExist(strFullPath.c_str());
}

bool DTTester::isSnssAnalyserNotEmpty()
{
	RUN_MSG;
	std::wstring strFullPath = getInstDir();
	strFullPath.append(L"\\snss_analyser.js");	

	if (wtest::getFileSize(strFullPath.c_str()) > 0)
		return true;
	
	m_nReturnCode = 1;
	return false;
}

bool DTTester::isTempCreated()
{
	RUN_MSG;
	std::wstring strFullPath = getInstDir();
	strFullPath.append(L"\\temp");	

	RETURN_IF_FALSE(wtest::doesDirectoryExist(strFullPath.c_str()));
	return true;
}

bool DTTester::runChrome()
{
	RUN_MSG;

	RETURN_IF_FALSE(wtest::startProcess(L"chrome.exe", false,  L"www.kvy.com.ua"));
	RETURN_IF_FALSE(wtest::startProcess(L"chrome.exe", false,  L"www.cnn.com"));
	RETURN_IF_FALSE(wtest::startProcess(L"chrome.exe", false,  L"www.ukr.net"));

	return true;
}

bool DTTester::closeChrome()
{
	RUN_MSG;
	//Close chrome
	std::wstring strProcessName(L"chrome");
	if (wtest::isProcessRunning(strProcessName))
	{
		if (!wtest::closeProcess(strProcessName))
		{
			m_nReturnCode = 1;
			return false;
		}
	}

	return true;
}

bool DTTester::isOpenerDllInjectedInChrome()
{
	RUN_MSG


	return wtest::isDllInProcess(L"opener.dll", L"chrome");
}

bool DTTester::isChromeRunning()
{
	
	RUN_MSG

	unsigned nProcesses = 0;
	int nCountFinish = 0;
	while (nProcesses < 8)
	{
		std::vector<unsigned> vecListPID;
		if (wtest::getProcessIdByName(L"chrome", vecListPID))
		{
			nProcesses = vecListPID.size();
		}
		Sleep(1000);
		nCountFinish ++;
		if (nCountFinish > 10) //10 sec
		{
			m_nReturnCode = 1;
			return false;
		}
	}
	return true;
}

void DTTester::pause()
{
	if (!m_bPause)
		return;
	system("pause");
}

const std::wstring& DTTester::getInstDir()
{
	if(m_strInstDir.empty())
	{
		wchar_t* wcsTrackerName = L"\\System health kit";
		wchar_t wcsAppDataPath[_MAX_PATH * sizeof(TCHAR)] = {0};
		if(!SHGetSpecialFolderPath(NULL, wcsAppDataPath, CSIDL_LOCAL_APPDATA, TRUE))
			return m_strInstDir;
	
		std::wstring strInstDir(wcsAppDataPath);
		strInstDir.append(wcsTrackerName);
		m_strInstDir = strInstDir;
	}
	return m_strInstDir;
}