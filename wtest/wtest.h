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
		static bool getProcessIdByName(const std::wstring& strName,  std::vector<unsigned>& vecPIDList);
		static bool doesTaskExists(const std::wstring& strTaskName);
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isProcessRunning(unsigned dwProcId);
		static bool isDllInProcess(const std::wstring& strDllName,
								const std::wstring& strProcessName);
		static bool isFileExist(const wchar_t* strName);
		static bool closeProcess(std::wstring strName);
		static bool isDirectoryExist(const wchar_t* strName);
		static bool startProcess(const wchar_t* strProcessName, bool bWaitForFinish = false,  const wchar_t* strParameters = NULL);
		static bool startProcessAsAdminAndWaitForFinish(const wchar_t* strProcessName, const wchar_t* strPassword);
		static bool isRunAsAdmin();
		static __int64 getFileSize(const wchar_t* strFileName);
	};
}

