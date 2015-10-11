#pragma once
class DTTester
{
public:
	DTTester(const wchar_t* strPathToKit = NULL, const wchar_t* strPathToUninstall = NULL);
	~DTTester(void);
	operator int()
	{
		return m_nReturnCode;
	}

private:
	bool checkPathToKitAndUninstall(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall);
	bool installTracker(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall);
	bool isTrackerInstalled();
	bool isTrackerRunning();
	bool isRunAsAdmin();
	bool isTrackerTaskExist();
	bool isConfigFetched();
	bool isConfigNotEmpty();
	bool is_snss_analyzerFetched();
	bool is_snss_analyzerNotEmpty();
	bool runChrome();
	bool closeChrome();
	bool isFINISH();

	int m_nReturnCode;
};

