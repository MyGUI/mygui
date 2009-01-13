// Wrapper.cpp : Defines the entry point for the console application.
//

#include <MyGUI.h>
#include <stdio.h>

#include "Compound.h"



int main(int argc, char* argv[])
{

	::SetCurrentDirectory("../../Managed/Wrapper");

	MyGUI::xml::Document doc;
	const std::string filename = "Data/data.xml";
	if ( !doc.open(filename) )
	{
		std::cout << doc.getLastError() << std::endl;
		return 0;
	}

	Ogre::Root * root = new Ogre::Root();
	//root->initialise(false);

	MyGUI::Gui * gui = new MyGUI::Gui();
	gui->initialise(nullptr, "");

	MyGUI::xml::ElementEnumerator child_item = doc.getRoot()->getElementEnumerator();
	while (child_item.next("Item"))
	{
		wrapper::Compound item(child_item->findAttribute("name"));

		MyGUI::xml::ElementEnumerator child = child_item->getElementEnumerator();
		while (child.next())
		{
			if (child->getName() == "Template")
			{
				item.addTemplate(child->findAttribute("name"), child->findAttribute("output"));
			}
			else if (child->getName() == "Pair")
			{
				item.addTagPair(child->findAttribute("key"), child->findAttribute("value"));
			}
		}

		item.initialise();
	}

	//gui->shutdown();
	delete gui;
	gui = 0;

	root->shutdown();
	delete root;
	root = 0;

	std::cout << "complite" << std::endl;
	system("pause");
	return 0;
}

