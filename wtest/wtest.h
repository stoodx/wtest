#pragma once
#include <string>
#include <vector>

namespace stoodx
{
	class wtest
	{
		wtest() {};
		unsigned GetProcessIDByName(const std::wstring& strName);
		bool FindProcessByName(const std::wstring& strName, unsigned& dwPID);
		int FindAllModulesOfProcess(unsigned dwPID, std::vector<std::wstring>* pvecListModules);

	public:
		~wtest() {};
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isDllInProcess(const std::wstring& strDllName,
							const std::wstring& strProcessName);
	};

	
}

