/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "FontPanel.h"

namespace demo
{

	FontPanel::FontPanel() : wraps::BaseLayout("FontPanel.layout")
	{
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

		mComboAntialias->setIndexSelected(1);
		mButtonGenerate->eventMouseButtonClick = MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);
		mButtonSave->eventMouseButtonClick = MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

		mFontName = "FontName";
		mFontHeight = 0;

		// загружаем демо вью
		mFontView = new FontView();
		mTextureView = new TextureView();

		update();
	}

	FontPanel::~FontPanel()
	{
		delete mFontView;
		delete mTextureView;
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
		if (mComboFont->getItemCount() > 0) mComboFont->setIndexSelected(0);
	}

	void FontPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _widget)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		root->addAttribute("type", "Font");

		MyGUI::xml::ElementPtr node = root->createChild("Font");
		node->addAttribute("name", mFontName);
		node->addAttribute("default_height", MyGUI::utility::toString(mFontHeight));
		node->addAttribute("source", mComboFont->getCaption());
		node->addAttribute("size", MyGUI::utility::parseInt(mEditSize->getCaption()));
		node->addAttribute("resolution", MyGUI::utility::parseInt(mEditResolution->getCaption()));
		node->addAttribute("antialias_colour", mComboAntialias->getCaption());
		node->addAttribute("space_width", MyGUI::utility::parseInt(mEditSpace->getCaption()));
		node->addAttribute("tab_width", MyGUI::utility::parseInt(mEditTab->getCaption()));
		node->addAttribute("cursor_width", MyGUI::utility::parseInt(mEditCursor->getCaption()));
		node->addAttribute("distance", MyGUI::utility::parseInt(mEditDistance->getCaption()));
		node->addAttribute("offset_height", MyGUI::utility::parseInt(mEditOffset->getCaption()));

		node->createChild("Code")->addAttribute("range", "33 126");
		node->createChild("Code")->addAttribute("range", "1025 1105");

		if (_widget == mButtonSave) {
			if (!document.save(mEditSaveFileName->getCaption())) {
				MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", document.getLastError(), "error save", MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
			}
			MyGUI::FontManager::getInstance().saveFontTexture(mFontName, mEditSaveFileName->getCaption() + ".png");
		}
		else if (_widget == mButtonGenerate) {

			// удаляем демо вью
			delete mFontView;
			mFontView = 0;
			delete mTextureView;
			mTextureView = 0;

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
			mFontView = new FontView();
			mFontView->setFontName(mFontName);

			mTextureView = new TextureView();
			mTextureView->setFontName(mFontName);
		}

	}

} // namespace demo
