/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
		assignWidget(mEditRange1, "edit_Range1");
		assignWidget(mEditRange2, "edit_Range2");
		assignWidget(mEditHide, "edit_Hide");
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

		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getDataListNames("*.ttf");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
		{
			std::string file = *iter;
			mComboFont->addItem(file);
		}
		if (mComboFont->getItemCount() > 0) mComboFont->setIndexSelected(0);
	}

	template<typename Type>
	void addProperty(MyGUI::xml::ElementPtr _node, const std::string& _name, Type _value)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Property");
		node->addAttribute("key", _name);
		node->addAttribute("value", _value);
	}

	void FontPanel::notifyMouseButtonClick(MyGUI::Widget* _widget)
	{
		// шрифтов нету
		if (mComboFont->getCaption().empty()) return;
		if (mEditSaveFileName->getCaption().empty() && _widget == mButtonSave) return;

		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		generateFontTTFXml(root);

		if (_widget == mButtonSave)
		{
			if (!document.save(mEditSaveFileName->getCaption() + ".xml"))
			{
				/*MyGUI::Message* message =*/ MyGUI::Message::createMessageBox("Message", "error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
			}
			else
			{
				/*MyGUI::Message* message =*/ MyGUI::Message::createMessageBox("Message", "success", mEditSaveFileName->getCaption() + ".xml", MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
			}

			MyGUI::IFont* font = MyGUI::FontManager::getInstance().getByName(mFontName);
			MyGUI::ITexture* texture = font->getTextureFont();
			texture->saveToFile(mEditSaveFileName->getCaption() + ".png");
		}
		else if (_widget == mButtonGenerate)
		{
			MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
			if (manager.isExist(mFontName))
			{
				manager.remove(mFontName);
			}

			MyGUI::ResourceManager::getInstance()._load(root, "", MyGUI::Version());
			MyGUI::IResource* resource = manager.getByName(mFontName, false);
			MYGUI_ASSERT(resource != nullptr, "Could not find font '" << mFontName << "'");
			MyGUI::IFont* font = resource->castType<MyGUI::IFont>();

			// вывод реального размера шрифта
			mFontHeight = font->getDefaultHeight();
			mTextPix->setCaption(MyGUI::utility::toString("Height of a font is ", mFontHeight, " pixels"));

			mFontView->setFontName(mFontName);
			mTextureView->setFontName(mFontName);
		}
	}

	void FontPanel::generateFontTTFXml(MyGUI::xml::ElementPtr _root)
	{
		_root->addAttribute("type", "Resource");
		_root->addAttribute("version", "1.1");

		MyGUI::xml::ElementPtr node = _root->createChild("Resource");
		node->addAttribute("type", "ResourceTrueTypeFont");
		node->addAttribute("name", mFontName);

		addProperty(node, "Source", mComboFont->getCaption());
		addProperty(node, "Size", MyGUI::utility::parseValue<int>(mEditSize->getCaption()));
		addProperty(node, "Resolution", MyGUI::utility::parseValue<int>(mEditResolution->getCaption()));
		addProperty(node, "Antialias", MyGUI::utility::parseValue<bool>(mComboAntialias->getCaption()));
		addProperty(node, "SpaceWidth", MyGUI::utility::parseValue<int>(mEditSpace->getCaption()));
		addProperty(node, "TabWidth", MyGUI::utility::parseValue<int>(mEditTab->getCaption()));
		addProperty(node, "CursorWidth", MyGUI::utility::parseValue<int>(mEditCursor->getCaption()));
		addProperty(node, "Distance", MyGUI::utility::parseValue<int>(mEditDistance->getCaption()));
		addProperty(node, "OffsetHeight", MyGUI::utility::parseValue<int>(mEditOffset->getCaption()));

		MyGUI::xml::ElementPtr node_codes = node->createChild("Codes");

		if (mEditRange1->getCaption() != "")
			node_codes->createChild("Code")->addAttribute("range", mEditRange1->getCaption());
		if (mEditRange2->getCaption() != "")
			node_codes->createChild("Code")->addAttribute("range", mEditRange2->getCaption());
		if (mEditHide->getCaption() != "")
			node_codes->createChild("Code")->addAttribute("hide", mEditHide->getCaption());
	}

} // namespace demo
