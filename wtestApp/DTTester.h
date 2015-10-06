#pragma once
class DTTester
{
public:
	DTTester(void);
	~DTTester(void);
	operator int()
	{
		return m_nReturnCode;
	}

private:
	bool installTracker();
	bool isTrackerInstalled();

	int m_nReturnCode;
};

