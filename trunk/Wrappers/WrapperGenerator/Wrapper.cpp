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

	std::cout << std::endl << "select command : " << std::endl << std::endl
		<< "0 - Exit" << std::endl
		<< "1 - generate solution" << std::endl
		<< "2 - doxygen" << std::endl
		<< "3 - Sharp" << std::endl
		<< "4 - Managed" << std::endl << std::endl;

	int num = 0;
	std::cin >> num;

	std::cout << std::endl;

	if (num != 1)
	{
		std::string folder = MYGUI_SOURCE_DIR;
		folder += "/Wrappers/WrapperGenerator";
		::SetCurrentDirectoryA(folder.c_str());
	}

	if (num == 1)
	{
		std::string template_sln = MYGUI_SOURCE_DIR;
		template_sln += "/Wrappers/WrapperGenerator/Data/Wrappers.sln.txt";

		wrapper::addTag("MyGUI_SourceDir", MYGUI_SOURCE_DIR);
		
		std::ifstream infile;
		infile.open(template_sln.c_str());
		if (infile.is_open())
		{
			std::ofstream outfile;
			std::string outfilename = "../../Wrappers/Wrappers.sln";
			outfile.open(outfilename.c_str());
			if (outfile.is_open())
			{
				std::string read;
				std::string data;

				while (!infile.eof())
				{
					std::getline(infile, read);
					read = wrapper::replaceTags(read);
					if (!data .empty()) data += "\n";
					data += read;
				}

				outfile << data;

				infile.close();
				outfile.close();
				std::cout << std::endl << "complete" << std::endl << std::endl;
			}
			else
			{
				std::cout << "error open file " << outfilename << std::endl;
				infile.close();
			}
		}
		else
		{
			std::cout << "error open file " << template_sln << std::endl;
		}
	}
	else if (num == 2)
	{
		::SetCurrentDirectoryA("doxygen");
		system("doxygen.exe");
	}
	else if (num == 3)
	{
		std::cout << std::endl << "start" << std::endl << std::endl;
		wrapper::Wrapper * wrap = new wrapper::Wrapper();
		wrap->initialise("Data/SharpData.xml");
		wrap->wrap();
		std::cout << std::endl << "complete" << std::endl << std::endl;
		delete wrap;
		wrap = 0;
	}
	else if (num == 4)
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

