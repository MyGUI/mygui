// Wrapper.cpp : Defines the entry point for the console application.
//

#include "windows.h"
#include <stdio.h>

#include "Wrapper.h"

#if WIN32
	int main(int argc, char **argv);
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
	void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
	void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif


int main(int argc, char **argv)
{

	std::string folder = MYGUI_SOURCE_DIR;
	folder += "/Wrapper/Wrapper";
	::SetCurrentDirectoryA(folder.c_str());

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
	}
	else if (num == 3)
	{
		std::cout << std::endl << "start" << std::endl << std::endl;
		wrapper::Wrapper * wrap = new wrapper::Wrapper();
		wrap->initialise("Data/ManagedData.xml");
		wrap->wrap();
		std::cout << std::endl << "complete" << std::endl << std::endl;
	}

	system("pause");
 	return 0;
}

