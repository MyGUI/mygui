// Wrapper.cpp : Defines the entry point for the console application.
//

#if WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "windows.h"
#include <stdio.h>

#include "Utility.h"
#include "Wrapper.h"

int main(int argc, char* argv[])
{

#if WIN32
	_CrtSetDbgFlag(_CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF/* | _CRTDBG_LEAK_CHECK_DF*/);
#endif

	wrapper::initialise();

	::SetCurrentDirectoryA("../../Wrapper/Wrapper");

	std::cout << std::endl << "select command : " << std::endl << std::endl
		<< "0 - Exit" << std::endl
		<< "1 - doxygen" << std::endl
		<< "2 - Sharp" << std::endl
		<< "3 - Managed" << std::endl << std::endl;

	int num = 0;
	std::cin >> num;

	std::cout << std::endl;

	if (num == 1)
	{
		::SetCurrentDirectoryA("doxygen");
		system("doxygen.exe");
	}
	else if (num == 2)
	{
		std::cout << std::endl << "start" << std::endl << std::endl;
		wrapper::Wrapper * wrap = new wrapper::Wrapper();
		wrap->initialise("Data/SharpData.xml");
		wrap->wrap();
		std::cout << std::endl << "complete" << std::endl << std::endl;
		delete wrap;
		wrap = 0;
	}
	else if (num == 3)
	{
		std::cout << std::endl << "start" << std::endl << std::endl;
		wrapper::Wrapper * wrap = new wrapper::Wrapper();
		wrap->initialise("Data/ManagedData.xml");
		wrap->wrap();
		std::cout << std::endl << "complete" << std::endl << std::endl;
		delete wrap;
		wrap = 0;
	}

	wrapper::shutdown();

#if WIN32
	_CrtDumpMemoryLeaks();
#endif

	system("pause");
	return 0;
}

