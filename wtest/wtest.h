#pragma once
#include <string>

namespace stoodx
{
	class wtest
	{
		wtest() {};
		int GetProcessIDByName(const std::wstring& strName);
		bool FindProcessByName(const std::wstring& strName);
	public:
		~wtest() {};
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isDllInProcess(const std::wstring& strDllName,
							const std::wstring& strProcessName);
	};

	
}

