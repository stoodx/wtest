#include "stdafx.h"
#include "DTTester.h"
#include <iostream>

#define DT_ASSERT(x) if(x == false) \
						{\
							std::cout << "[    FAIL] " << std::endl;\
							return;\
						}\
						else\
							std::cout << "[      OK] " << std::endl;\

DTTester::DTTester(void)
	: m_nReturnCode(0)
{
	// 1. Install the tracker
	// 2. Check if tracker is installed
	// 3. Check if tracker is running
	// 4. Check if tracker task exists
	// 10. Check if config was fetched
	// 11. Check that config is not empty
	// 12. Check that snss_analyzer is fetched
	// 13. Check that snss_analyzer is not empty

	// 5. Run chrome ( with prev tabs )
	// 6. Check if chrome is running
	// 7. Check if chrome has opener.dll
	// 8. Close chrome

	// 9. Check if folder "temp" was created
	DT_ASSERT(installTracker());
	DT_ASSERT(isTrackerInstalled());
}

DTTester::~DTTester(void)
{
}

bool DTTester::installTracker()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;
	return true;
}

bool DTTester::isTrackerInstalled()
{
	std::cout << "[RUN     ] " << __FUNCTION__ << std::endl;
	return false;
}
