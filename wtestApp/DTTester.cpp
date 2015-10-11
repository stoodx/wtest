#include "stdafx.h"
#include <iostream>
#include <ShlObj.h>

#include "../wtest/wtest.h"
#include "DTTester.h"

using namespace stoodx;

#define DT_ASSERT(x) if(x == false) \
						{\
							std::cout << "[    FAIL] " << std::endl;\
							return;\
						}\
						else\
							std::cout << "[      OK] " << std::endl;\

DTTester::DTTester(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall)
	: m_nReturnCode(0)
{
	//0. Is run as Admin
	DT_ASSERT(isRunAsAdmin());
	//1. Check path to kit and install
	DT_ASSERT(checkPathToKitAndUninstall(strPathToKit, strPathToUninstall));
	//2. Install the tracker
	DT_ASSERT(installTracker(strPathToKit, strPathToUninstall));
	//3. Check if tracker is installed
	DT_ASSERT(isTrackerInstalled());	
	//4. Check if tracker is running
	DT_ASSERT(isTrackerRunning());
	//5. Check if tracker task exists
	DT_ASSERT(isTrackerTaskExist());
	//6. Check if config was fetched
	DT_ASSERT(isConfigFetched()); //not released
	//7. Check that config is not empty
	DT_ASSERT(isConfigNotEmpty()); //not released
	//8. Check that snss_analyzer is fetched
	DT_ASSERT(is_snss_analyzerFetched());
	//9. Check that snss_analyzer is not empty
	DT_ASSERT(is_snss_analyzerNotEmpty());
	//10. Run chrome ( with prev tabs )
	DT_ASSERT(runChrome());
	//11. Check if chrome has opener.dll
	
	//12. Close chrome
	//Sleep(10000);
	//DT_ASSERT(closeChrome());

	// 9. Check if folder "temp" was created

	DT_ASSERT(isFINISH());
}

DTTester::~DTTester(void)
{
}

bool DTTester::installTracker(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall)
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;
	//chrome iis run?
	std::wstring strProcessName(L"chrome");
	if (wtest::isProcessRunning(strProcessName))
	{//close chrome
		if (!wtest::closeProcess(strProcessName))
		{
			m_nReturnCode = 1;
			return false;
		}
	}

	if (isTrackerInstalled())
	{//uninstall kit
		if (!wtest::startProcess(strPathToUninstall, true))
		{
			m_nReturnCode = 1;
			return false;
		}
	}
	//install kit
	if (!wtest::startProcess(strPathToKit, true))
	{
		m_nReturnCode = 1;
		return false;
	}

	return true;
}

bool DTTester::isTrackerInstalled()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;
	//define directory of kit
	wchar_t* strKitName = L"System health kit";
	wchar_t strP[_MAX_PATH * sizeof(TCHAR)] = {0};
	if (!SHGetSpecialFolderPath(NULL, strP, CSIDL_LOCAL_APPDATA, TRUE))
	{
		m_nReturnCode = 1;
		return false;
	}
	std::wstring strFullPath(strP);
	strFullPath += L"\\" ; 
	strFullPath += strKitName;
	
	if (!wtest::isDirectoryExist(strFullPath.c_str()))
		return false;
	strFullPath += L"\\";
	std::wstring  strOpener_dll(strFullPath), strSystem_health_kit_exe(strFullPath);
	strOpener_dll.append(L"Opener.dll");
	strSystem_health_kit_exe.append(L"System health kit.exe");
	if (!wtest::isFileExist(strOpener_dll.c_str()) || 
		!wtest::isFileExist(strSystem_health_kit_exe.c_str()))
		return false;

	return true;
}

bool DTTester::checkPathToKitAndUninstall(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall)
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	if (!wtest::isFileExist(strPathToKit) ||
		!wtest::isFileExist(strPathToUninstall))
	{
		m_nReturnCode = 1;	
		return false;
	}

	return true;
}

bool DTTester::isTrackerRunning()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	std::wstring strTrackerName(L"System health kit.exe");
	return wtest::isProcessRunning(strTrackerName);
}

bool DTTester::isRunAsAdmin()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	return wtest::isRunAsAdmin();
}

bool DTTester::isTrackerTaskExist()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	std::wstring strTaskName(L"System Health Kit");
	return wtest::doesTaskExists(strTaskName);
}

bool DTTester::isConfigFetched()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	return true;
}

bool DTTester::isConfigNotEmpty()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	return true;
}

bool DTTester::is_snss_analyzerFetched()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	//define directory of kit
	wchar_t* strKitName = L"System health kit";
	wchar_t strP[_MAX_PATH * sizeof(TCHAR)] = {0};
	if (!SHGetSpecialFolderPath(NULL, strP, CSIDL_LOCAL_APPDATA, TRUE))
	{
		m_nReturnCode = 1;
		return false;
	}
	std::wstring strFullPath(strP);
	strFullPath += L"\\" ; 
	strFullPath += strKitName;
	if (!wtest::isDirectoryExist(strFullPath.c_str()))
		return false;
	strFullPath += L"\\";
	strFullPath += L"snss_analyser.js";	

	return wtest::isFileExist(strFullPath.c_str());
}

bool DTTester::is_snss_analyzerNotEmpty()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	//define directory of kit
	wchar_t* strKitName = L"System health kit";
	wchar_t strP[_MAX_PATH * sizeof(TCHAR)] = {0};
	if (!SHGetSpecialFolderPath(NULL, strP, CSIDL_LOCAL_APPDATA, TRUE))
	{
		m_nReturnCode = 1;
		return false;
	}
	std::wstring strFullPath(strP);
	strFullPath += L"\\" ; 
	strFullPath += strKitName;
	if (!wtest::isDirectoryExist(strFullPath.c_str()))
		return false;
	strFullPath += L"\\";
	strFullPath += L"snss_analyser.js";	

	if (wtest::getFileSize(strFullPath.c_str()) > 0)
		return true;
	return false;
}

bool DTTester::runChrome()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	if (!wtest::startProcess(L"chrome.exe", false,  L"www.kvy.com.ua www.cnn.com www.ukr.net"))
		return false;

	return true;
}

bool DTTester::isFINISH()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	return true;
}

bool DTTester::closeChrome()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;

	return wtest::closeProcess(L"chrome");
}