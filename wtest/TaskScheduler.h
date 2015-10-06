#pragma once
#include <string>

namespace stoodx
{
	class TaskScheduler
	{
	public:
		TaskScheduler(void);
		~TaskScheduler(void);

		bool doesTaskExists(const std::wstring& strTaskName);
	};

}