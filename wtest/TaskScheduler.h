#pragma once
#include <string>

class TaskScheduler
{
public:
	TaskScheduler(void);
	~TaskScheduler(void);

	bool doesTaskExists(const std::wstring& strTaskName);
};

