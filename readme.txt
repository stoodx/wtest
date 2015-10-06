1. static bool isProcessRunning(const std::wstring& strProcessName);
		

Usage:
	strProcessName can be in low/upper cases, with/without ".exe" 

2. static bool isDllInProcess(const std::wstring& strDllName,
							
				const std::wstring& strProcessName);

Usage:
	- strProcessName  and strDllName can be in low/upper cases, with/without ".exe"  and ".dll"
	- if it starts in 32-bit environment, then the function sees only 32-bit DLLs.  

3. bool wtest::isProcessRunning(unsigned dwProcId)