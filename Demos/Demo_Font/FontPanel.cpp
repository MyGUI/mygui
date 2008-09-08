/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "FontPanel.h"

FontPanel::FontPanel() :
	BaseLayout("FontPanel.layout")
{
}

void FontPanel::initialise()
{
	loadLayout();
	assignWidget(mComboFont, "combo_Fonts");
	assignWidget(mEditSize, "edit_Size");
	assignWidget(mEditResolution, "edit_Resolution");
	assignWidget(mComboAntialias, "combo_Antialias");
	assignWidget(mEditSpace, "edit_Space");
	assignWidget(mEditTab, "edit_Tab");
	assignWidget(mEditCursor, "edit_Cursor");
	assignWidget(mEditDistance, "edit_Distance");
	assignWidget(mEditOffset, "edit_Offset");
	assignWidget(mButtonGenerate, "button_Generate");
	assignWidget(mEditDemo, "edit_Demo");
	assignWidget(mTextPix, "text_Pix");

	mComboAntialias->setItemSelect(1);
	mButtonGenerate->eventMouseButtonClick = MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

	update();
}

void FontPanel::show()
{
	mMainWidget->show();
}

void FontPanel::hide()
{
	mMainWidget->hide();
}

void FontPanel::update()
{
	mComboFont->deleteAllItems();

	MyGUI::helper::VectorString paths = MyGUI::helper::getVectorResourcePath("*.ttf");
	for (MyGUI::helper::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter) {
		std::string file = *iter;
		size_t pos = file.find_last_of("\\/");
		if (pos != std::string::npos) file = file.substr(pos + 1);
		mComboFont->addItem(file);
	}
	if (mComboFont->getItemCount() > 0) mComboFont->setItemSelect(0);
}

void FontPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _widget)
{
	MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
	if (manager.isExist("font_tmp")) {
		manager.remove("font_tmp");
	}

	MyGUI::xml::xmlDocument document;
	document.createInfo();
	MyGUI::xml::xmlNodePtr root = document.createRoot("MyGUI");
	root->addAttributes("type", "Font");

	MyGUI::xml::xmlNodePtr node = root->createChild("Font");
	node->addAttributes("name", "font_tmp");
	node->addAttributes("default_height", "11");
	node->addAttributes("source", mComboFont->getCaption());
	node->addAttributes("size", MyGUI::utility::parseInt(mEditSize->getCaption()));
	node->addAttributes("resolution", MyGUI::utility::parseInt(mEditResolution->getCaption()));
	node->addAttributes("antialias_colour", mComboAntialias->getCaption());
	node->addAttributes("space_width", MyGUI::utility::parseInt(mEditSpace->getCaption()));
	node->addAttributes("tab_width", MyGUI::utility::parseInt(mEditTab->getCaption()));
	node->addAttributes("cursor_width", MyGUI::utility::parseInt(mEditCursor->getCaption()));
	node->addAttributes("distance", MyGUI::utility::parseInt(mEditDistance->getCaption()));
	node->addAttributes("offset_height", MyGUI::utility::parseInt(mEditOffset->getCaption()));

	node->createChild("Code")->addAttributes("range", "33 126");
	node->createChild("Code")->addAttributes("range", "1025 1105");

	manager._load(root, "");

	mEditDemo->setFontName("font_tmp");
	int height = static_cast<MyGUI::FontPtr>(manager.getByName("font_tmp"))->getHeightPix();
	mEditDemo->setFontHeight(height);
	mTextPix->setCaption(MyGUI::utility::toString("Height of a font of ", height, " pixels"));
}

