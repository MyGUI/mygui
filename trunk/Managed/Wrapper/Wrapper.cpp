// Wrapper.cpp : Defines the entry point for the console application.
//

#include <MyGUI.h>
#include <stdio.h>

#include "Wrapper.h"

int main(int argc, char* argv[])
{

	::SetCurrentDirectoryA("../../Managed/Wrapper");

	Ogre::Root * root = new Ogre::Root();
	//root->initialise(false);

	MyGUI::Gui * gui = new MyGUI::Gui();
	gui->initialise(nullptr, "");

	std::cout << std::endl << "start" << std::endl << std::endl;
	wrapper::Wrapper * wrap = new wrapper::Wrapper();
	wrap->initialise();

	wrap->wrap();

	//gui->shutdown();
	delete gui;
	gui = 0;

	//root->shutdown();
	delete root;
	root = 0;

	std::cout << std::endl << "complite" << std::endl << std::endl;
	system("pause");
	return 0;
}

