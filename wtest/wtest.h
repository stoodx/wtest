#pragma once
#include <string>

namespace stoodx
{
	class wtest
	{
		wtest() {};
		bool FindProcessByName(const std::wstring& strName);
	public:
		~wtest() {};
		static int getProcessIdByName(const std::wstring& strName);
		static bool doesTaskExists(const std::wstring& strTaskName);
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isProcessRunning(unsigned long dwProcId);
		static bool isDllInProcess(const std::wstring& strDllName,
							const std::wstring& strProcessName);
	};
}

