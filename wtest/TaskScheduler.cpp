#include "TaskScheduler.h"
#include <taskschd.h>
#include <string>
#include <thread>
#include <comdef.h>
#include <tchar.h>

# pragma comment(lib, "taskschd.lib")

TaskScheduler::TaskScheduler(void)
{
}


TaskScheduler::~TaskScheduler(void)
{
}

bool TaskScheduler::doesTaskExists(const std::wstring& strTaskName)
{
	bool bRes = false;
	std::thread t( [&] ()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if( FAILED(hr)  && hr != RPC_E_CHANGED_MODE)
		{
			return;
		}

		//  Set general COM security levels.
		hr = CoInitializeSecurity(
				NULL,
				-1,
				NULL,
				NULL,
				RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
				RPC_C_IMP_LEVEL_IMPERSONATE,
				NULL,
				0,
				NULL);

		//  Create an instance of the Task Service. 
		ITaskService *pService = NULL;
		hr = CoCreateInstance( CLSID_TaskScheduler,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_ITaskService,
				(void**)&pService );  
		if (FAILED(hr)) {
			CoUninitialize();
			return;
		}

		//  Connect to the task service.
		hr = pService->Connect(_variant_t(), _variant_t(),
				_variant_t(), _variant_t());
		if( FAILED(hr) ) {
			pService->Release();
			CoUninitialize();
			return;
		}

		//  Get the pointer to the root task folder.
		ITaskFolder *pRootFolder = NULL;
		hr = pService->GetFolder( _bstr_t( L"\\") , &pRootFolder );

		pService->Release();
		if( FAILED(hr) ) {
			CoUninitialize();
			return;
		}

		//  Get the registered tasks in the folder.
		IRegisteredTaskCollection* pTaskCollection = NULL;
		hr = pRootFolder->GetTasks( NULL, &pTaskCollection );

		pRootFolder->Release();
		if( FAILED(hr) ) {
			CoUninitialize();
			return;
		}

		LONG numTasks = 0;
		hr = pTaskCollection->get_Count(&numTasks);

		if( numTasks == 0 ) {
			pTaskCollection->Release();
			CoUninitialize();
			return;
		}

		for(LONG i=0; i < numTasks; i++) {
			IRegisteredTask* pRegisteredTask = NULL;
			hr = pTaskCollection->get_Item( _variant_t(i+1), &pRegisteredTask );

			if( SUCCEEDED(hr) ) {
				TCHAR* taskNameFromListOfUserTasks = NULL;
				hr = pRegisteredTask->get_Name(&taskNameFromListOfUserTasks);
				if( SUCCEEDED(hr) ) {
					bool isMatch = (_tcscmp(taskNameFromListOfUserTasks, strTaskName.c_str())==0);
					if(isMatch) {

						pTaskCollection->Release();
						pRegisteredTask->Release();
						CoUninitialize();
						SysFreeString(taskNameFromListOfUserTasks);
						bRes = true;
						return;

					}               
				} else {
					SysFreeString(taskNameFromListOfUserTasks);
				}
			} 
			pRegisteredTask->Release();
		}

		CoUninitialize();
		return;
	});
	t.join();
	return bRes;
}