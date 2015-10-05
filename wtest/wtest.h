#include <string>

namespace stoodx
{
	class wtest
	{
		static bool isProcessRunning(const std::wstring& strProcessName);
		static bool isDllInProcess(const std::wstring& strDllName,
							const std::wstring& strProcessName);
	};
}