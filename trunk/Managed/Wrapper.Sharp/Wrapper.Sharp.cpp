// Wrapper.cpp : Defines the entry point for the console application.
//

#include "windows.h"
#include <stdio.h>

#include "Wrapper.Sharp.h"

int main(int argc, char* argv[])
{

	::SetCurrentDirectoryA("../../Managed/Wrapper.Sharp");

	std::cout << std::endl << "start" << std::endl << std::endl;
	wrapper::Wrapper * wrap = new wrapper::Wrapper();
	wrap->initialise();

	wrap->wrap();


	std::cout << std::endl << "complite" << std::endl << std::endl;
	system("pause");
	return 0;
}

