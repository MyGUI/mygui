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

		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getVectorDataPath("*.ttf"/*, MyGUI::ResourceManager::getInstance().getResourceGroup()*/);
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
		{
			std::string file = *iter;
			size_t pos = file.find_last_of("\\/");
			if (pos != std::string::npos) file = file.substr(pos + 1);
			mComboFont->addItem(file);
		}
		if (mComboFont->getItemCount() > 0) mComboFont->setIndexSelected(0);
	}

	void FontPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _widget)
	{
		// шрифтов нету
		if (mComboFont->getCaption().empty()) return;
		if (mEditSaveFileName->getCaption().empty() && _widget == mButtonSave) return;

		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		root->addAttribute("type", "Font");

		MyGUI::xml::ElementPtr node = root->createChild("Font");
		node->addAttribute("name", mFontName);
		//node->addAttribute("default_height", MyGUI::utility::toString(mFontHeight));
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

		if (_widget == mButtonSave)
		{
			if (!document.save(mEditSaveFileName->getCaption()))
			{
				MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", "error save", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
			}

			MyGUI::IFont* font = MyGUI::FontManager::getInstance().getByName(mFontName);
			MyGUI::ITexture* texture = font->getTextureFont();
			texture->saveToFile(mEditSaveFileName->getCaption() + ".png");
		}
		else if (_widget == mButtonGenerate)
		{
			MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
			if (manager.isExist(mFontName))
			{
				manager.remove(mFontName);
			}

			manager._load(root, "", MyGUI::Version());
			MyGUI::IFont* font = manager.getByName(mFontName);
			MYGUI_ASSERT(font != nullptr, "Could not find font '" << mFontName << "'");

			// вывод реального размера шрифта
			mFontHeight = font->getDefaultHeight();
			mTextPix->setCaption(MyGUI::utility::toString("Height of a font of ", mFontHeight, " pixels"));

			mFontView->setFontName(mFontName);
			mTextureView->setFontName(mFontName);
		}
	}

} // namespace demo
