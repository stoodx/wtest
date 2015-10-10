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
	bool checkPathToKitAndInstall(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall);
	bool installTracker(const wchar_t* strPathToKit, const wchar_t* strPathToUninstall);
	bool isTrackerInstalled();
	bool isTrackerRunning();


	int m_nReturnCode;
};

