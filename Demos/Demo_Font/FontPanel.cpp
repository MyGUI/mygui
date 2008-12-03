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
	assignWidget(mTextPix, "text_Pix");
	assignWidget(mEditSaveFileName, "edit_SaveFileName");
	assignWidget(mButtonSave, "button_Save");

	mComboAntialias->setItemSelectedAt(1);
	mButtonGenerate->eventMouseButtonClick = MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);
	mButtonSave->eventMouseButtonClick = MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

	mFontName = "FontName";
	mFontHeight = 0;

	// загружаем демо вью
	mFontView.initialise();
	mTextureView.initialise();

	update();
}

void FontPanel::update()
{
	mComboFont->removeAllItems();

	MyGUI::helper::VectorString paths = MyGUI::helper::getVectorResourcePath("*.ttf");
	for (MyGUI::helper::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter) {
		std::string file = *iter;
		size_t pos = file.find_last_of("\\/");
		if (pos != std::string::npos) file = file.substr(pos + 1);
		mComboFont->addItem(file);
	}
	if (mComboFont->getItemCount() > 0) mComboFont->setItemSelectedAt(0);
}

void FontPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _widget)
{
	MyGUI::xml::xmlDocument document;
	document.createInfo();
	MyGUI::xml::xmlNodePtr root = document.createRoot("MyGUI");
	root->addAttributes("type", "Font");

	MyGUI::xml::xmlNodePtr node = root->createChild("Font");
	node->addAttributes("name", mFontName);
	node->addAttributes("default_height", MyGUI::utility::toString(mFontHeight));
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
		
	if (_widget == mButtonSave) {
		if (!document.save(mEditSaveFileName->getCaption())) {
			MyGUI::Message::createMessage(document.getLastError(), "error save", true, MyGUI::Message::Ok | MyGUI::Message::IconError);
		}
		MyGUI::FontManager::getInstance().saveFontTexture(mFontName, mEditSaveFileName->getCaption() + ".png");
	}
	else if (_widget == mButtonGenerate) {
		// удаляем демо вью
		mFontView.shutdown();
		mTextureView.shutdown();

		MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
		if (manager.isExist(mFontName)) {
			manager.remove(mFontName);
		}

		manager._load(root, "", MyGUI::Version());
		MyGUI::FontPtr font = manager.getByName(mFontName);
		if (font.isNull()) MYGUI_EXCEPT("Could not find font '" << mFontName << "'");
		font->load();

		// вывод реального размера шрифта
		mFontHeight = static_cast<MyGUI::FontPtr>(manager.getByName(mFontName))->getHeightPix();
		mTextPix->setCaption(MyGUI::utility::toString("Height of a font of ", mFontHeight, " pixels"));

		// заново загружаем демо вью
		mFontView.initialise();
		mFontView.update(mFontName);
		mTextureView.initialise();
		mTextureView.update(mFontName);
	}

}

