#pragma once
class DTTester
{
public:
	DTTester(const std::wstring& strInstallerPath,
			 const std::wstring& strUnistallerPath = L"",
			 bool bStepByStep = false);
	
	DTTester();

	~DTTester(void);
	operator int()
	{
		return m_nReturnCode;
	}

	bool installTracker(const std::wstring& strInstallerPath);
	bool uninstallTracker(const std::wstring& strUninstallerPath);
	bool isTrackerInstalled();
	bool isTrackerRunning();
	bool doesTrackerTaskExist();
	bool isConfigFetched();
	bool isConfigNotEmpty();
	bool isSnssAnalyserFetched();
	bool isSnssAnalyserNotEmpty();
	bool isChromeRunning();
	bool isOpenerDllInjectedInChrome();
	bool runChrome();
	bool closeChrome();
	bool isTempCreated();

private:
	bool checkParams(void);
	bool isRunAsAdmin();
	void pause();
	const std::wstring& getInstDir();

	int m_nReturnCode;
	bool m_bPause;
	std::wstring m_strInstallerPath;
	std::wstring m_strUninstallerPath;
	std::wstring m_strInstDir;

};

