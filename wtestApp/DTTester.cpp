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
	//1. check path to kit and install
	DT_ASSERT(checkPathToKitAndInstall(strPathToKit, strPathToUninstall));
	// 2. Install the tracker
	DT_ASSERT(installTracker(strPathToKit, strPathToUninstall));
	// 3. Check if tracker is installed
	DT_ASSERT(isTrackerInstalled());	
	

	// 3. Check if tracker is running
	// 4. Check if tracker task exists
	// 10. Check if config was fetched
	// 11. Check that config is not empty
	// 12. Check that snss_analyzer is fetched
	// 13. Check that snss_analyzer is not empty

	// 5. Run chrome ( with prev tabs )
	// 6. Check if chrome is running
	// 7. Check if chrome has opener.dll
	// 8. Close chrome

	// 9. Check if folder "temp" was created

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
	}
	else
		m_nReturnCode = 0;

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
	else
		return true;
}

bool DTTester::checkPathToKitAndInstall(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall)
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