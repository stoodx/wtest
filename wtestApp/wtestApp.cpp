// wtestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../wtest/wtest.h"
#include "DTTester.h"
#include <gtest\gtest.h>
using namespace stoodx;


int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	int nRes = RUN_ALL_TESTS();
	system("pause");
	return nRes;
}

