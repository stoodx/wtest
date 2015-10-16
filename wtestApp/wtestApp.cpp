// wtestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../wtest/wtest.h"
#include "DTTester.h"

using namespace stoodx;


int _tmain(int argc, _TCHAR* argv[])
{

	int nResult = DTTester(argv[1], argv[2], argv[3]);

	//std::system("PAUSE");
	return nResult;
}

