/*!
	@file
	@author		Georgiy Evmenov
	@date		04/2009
	@module
*/

#include "precompiled.h"
#include "MetaSolutionWindow.h"
#include "BasisManager.h"

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

MetaSolutionWindow::MetaSolutionWindow() :
	BaseLayout("MetaSolutionWindow.layout")
{
	assignWidget(mListTree, "ListTree");

	mListTree->eventListSelectAccept = MyGUI::newDelegate(this, &MetaSolutionWindow::notifyListSelectAccept);
}

void MetaSolutionWindow::load(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
	while (field.next()) {
		/*std::string key, value;

		if (field->getName() == "Property")
		{
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;

			if (key == "key1") ;//do sth
			else if (key == "key2") ;//do sth
		}*/
	}
}

void MetaSolutionWindow::save(MyGUI::xml::ElementPtr root)
{
	/*root = root->createChild("MetaSolutionWindow");
	MyGUI::xml::ElementPtr nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "Grid");
	nodeProp->addAttribute("value", grid_step);

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "ShowName");
	nodeProp->addAttribute("value", getShowName());

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "ShowType");
	nodeProp->addAttribute("value", getShowType());*/
}

void MetaSolutionWindow::notifyListSelectAccept(MyGUI::ListPtr _sender, size_t _index)
{
}