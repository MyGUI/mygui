// Wrapper.cpp : Defines the entry point for the console application.
//

#include "windows.h"
#include <stdio.h>

#include "Wrapper.h"

int main(int argc, char* argv[])
{

	::SetCurrentDirectoryA("../../Wrapper/Wrapper");

	std::cout << std::endl << "start" << std::endl << std::endl;
	wrapper::Wrapper * wrap = new wrapper::Wrapper();
	wrap->initialise("Data/SharpData.xml");

	wrap->wrap();


	std::cout << std::endl << "complite" << std::endl << std::endl;
	system("pause");
	return 0;

}

