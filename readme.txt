1. static bool isProcessRunning(const std::wstring& strProcessName);
		

Usage:
	strProcessName can be in low/upper cases, with/without ".exe" 

2. static bool isDllInProcess(const std::wstring& strDllName,
							
				const std::wstring& strProcessName);

Usage:
	strProcessName  and strDllName can be in low/upper cases, with/without ".exe" 