#pragma once
#include <string>
#include <vector>

namespace stoodx
{
	class wtest
	{
		wtest() {};
		bool FindProcessByName(const std::wstring& strName,  std::vector<unsigned>& vecPIDList);
		int FindAllModulesOfProcess(unsigned dwPID, std::vector<std::wstring>* pvecListModules);
		bool StartProcessAndWaitForFinish(const wchar_t* strProcessName);
		bool StartProcessAndContinue(const wchar_t* strProcessName, const wchar_t* strParameters);

	public:
		~wtest() {};
		//processes
		static bool isRunAsAdmin();
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isProcessRunning(unsigned dwProcId);
		static bool isDllInProcess(const std::wstring& strDllName,	const std::wstring& strProcessName);
		static bool getProcessIdByName(const std::wstring& strName,  std::vector<unsigned>& vecPIDList);
		static bool closeProcess(std::wstring strName);
		static bool closeProcess(unsigned dwPID);
		static bool startProcess(const wchar_t* strProcessName, bool bWaitForFinish = false,  const wchar_t* strParameters = NULL);
		static bool startProcessAsAdminAndWaitForFinish(const wchar_t* strProcessName, int nTimeForWait_ms = 0);
		//files and directories
		static bool isFileExist(const wchar_t* strName);
		static bool isDirectoryExist(const wchar_t* strName);
		static __int64 getFileSize(const wchar_t* strFileName);
		//other
		static void Wait(int nTimeForWait_ms = 0);
		static bool doesTaskExists(const std::wstring& strTaskName);
	};
}

