//////////////////////Run//////////////////////////
Run with admin rights:

wtestApp.exe full_path_to_kit full_path_to_unistall

Example,
	wtestApp "System health kit 1.2.15.100617-beta.exe"  uninstal.bat

!!!!You should use uninstal.bat from the root directory

////////////////////////////////////////////////

1. bool isProcessRunning(const std::wstring& strProcessName);
		

Usage:
	strProcessName can be in low/upper cases, with/without ".exe" 

2. bool isDllInProcess(const std::wstring& strDllName,
							
				const std::wstring& strProcessName);

Usage:
	- strProcessName  and strDllName can be in low/upper cases, with/without ".exe"  and ".dll"
	- if it starts in 32-bit environment, then the function sees only 32-bit DLLs.  

3. bool wtest::isProcessRunning(unsigned dwProcId)

4. bool isFileExist(const wchar_t* strName)

5. bool closeProcess(const wchar_t* strName)

6. bool wtest::isDirectoryExist(const wchar_t* strName)

7. bool wtest::startProcess(const wchar_t* strProcessName, bool bWaitForFinish)

8. bool wtest::isRunAsAdmin()

9. __int64 getFileSize(const wchar_t* strFileName)