/*!
	@file
	@author		Georgiy Evmenov
	@date		01/2009
	@module
*/

#include "precompiled.h"
#include "SaveLoadWindow.h"
#include "BasisManager.h"

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

SaveLoadWindow::SaveLoadWindow() :
	BaseLayout("SaveLoadWindow.layout"),
	mSave(false)
{
	assignWidget(mComboFileName, "ComboFileName");
	assignWidget(mButtonOk, "ButtonOk");
	assignWidget(mButtonCancel, "ButtonCancel");

	mButtonOk->eventMouseButtonClick = MyGUI::newDelegate(this, &SaveLoadWindow::notifyOk);
	mButtonCancel->eventMouseButtonClick = MyGUI::newDelegate(this, &SaveLoadWindow::notifyCancel);
}

void SaveLoadWindow::load(MyGUI::xml::ElementEnumerator _field)
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

void SaveLoadWindow::save(MyGUI::xml::ElementPtr root)
{
	/*root = root->createChild("SaveLoadWindow");
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

void SaveLoadWindow::setSave(std::string _fileName)
{
	mButtonOk->setCaption(localise("Save"));

	if (_fileName != "") {
		mComboFileName->setCaption(_fileName);
	}

	setVisible(true);

	mSave = true;
}

void SaveLoadWindow::setLoad(std::string _fileName)
{
	mButtonOk->setCaption(localise("Load"));

	if (_fileName != "") {
		mComboFileName->setCaption(_fileName);
	}

	setVisible(true);

	mSave = false;
}

void SaveLoadWindow::setVisible(bool _visible)
{
	if (_visible)
	{
		// put in center
		MyGUI::IntSize view(MyGUI::Gui::getInstance().getViewSize());
		MyGUI::IntSize size(mMainWidget->getSize());
		mMainWidget->setCoord((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);

		// load list of *.layout files in combo
		std::vector<Ogre::String> strs = MyGUI::helper::getVectorResourcePath("*.layout");
		for (std::vector<Ogre::String>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
		{
			mComboFileName->addItem(*iter);
		}

		// make it modal
		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
	}
	else
	{
		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	mMainWidget->setVisible(_visible);
}

void SaveLoadWindow::notifyOk(MyGUI::WidgetPtr _sender)
{
	Ogre::UTFString file_name = mComboFileName->getCaption();

	if (mSave) eventSaveFile(file_name);
	else eventLoadFile(file_name);
}

void SaveLoadWindow::notifyCancel(MyGUI::WidgetPtr _sender)
{
	setVisible(false);
}
