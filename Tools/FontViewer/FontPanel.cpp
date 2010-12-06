/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "FontPanel.h"
#include "MessageBox/MessageBox.h"

namespace demo
{

	FontPanel::FontPanel() :
		wraps::BaseLayout("FontPanel.layout")
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
		mButtonGenerate->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);
		mButtonSave->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

		mFontName = "GeneratedFontName";
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
		for (MyGUI::VectorString::iterator iter = paths.begin(); iter != paths.end(); ++iter)
		{
			std::string file = *iter;
			mComboFont->addItem(file);
		}

		paths = MyGUI::DataManager::getInstance().getDataListNames("*.pfb");
		for (MyGUI::VectorString::iterator iter = paths.begin(); iter != paths.end(); ++iter)
		{
			std::string file = *iter;
			mComboFont->addItem(file);
		}

		if (mComboFont->getItemCount() > 0)
			mComboFont->setIndexSelected(0);
	}

	template<typename Type>
	void addProperty(MyGUI::xml::ElementPtr _node, const std::string& _name, Type _value)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Property");
		node->addAttribute("key", _name);
		node->addAttribute("value", _value);
	}

	void FontPanel::saveTexture(const std::string& _fontName, const std::string& _fileName)
	{
		MyGUI::IFont* font = MyGUI::FontManager::getInstance().getByName(_fontName);
		MyGUI::ITexture* texture = font->getTextureFont();
		texture->saveToFile(_fileName);
	}

	void FontPanel::saveFontTTFXml(const std::string& _fontName, const std::string& _fileName)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		generateFontTTFXml(root, _fontName);

		if (document.save(_fileName))
			MyGUI::Message::createMessageBox("Message", "success", _fileName, MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::saveFontManualXml(const std::string& _fontName, const std::string& _textureName, const std::string& _fileName)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		generateFontManualXml(root, _textureName, _fontName);

		if (document.save(_fileName))
			MyGUI::Message::createMessageBox("Message", "success", _fileName, MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::notifyMouseButtonClick(MyGUI::Widget* _widget)
	{
		// шрифтов нету
		if (mComboFont->getOnlyText().empty())
			return;
		if (mEditSaveFileName->getOnlyText().empty() && _widget == mButtonSave)
			return;

		if (_widget == mButtonSave)
		{
			std::string textureName = mEditSaveFileName->getOnlyText() + ".png";
			saveTexture(mFontName, textureName);

			std::string fontName = MyGUI::utility::toString(mEditSaveFileName->getOnlyText(), ".ttf.", mFontHeight);
			std::string fileName = mEditSaveFileName->getOnlyText() + ".ttf.xml";
			saveFontTTFXml(fontName, fileName);

			fontName = MyGUI::utility::toString(mEditSaveFileName->getOnlyText(), ".manual.", mFontHeight);
			fileName = mEditSaveFileName->getOnlyText() + ".manual.xml";
			saveFontManualXml(fontName, textureName, fileName);
		}
		else if (_widget == mButtonGenerate)
		{
			MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
			if (manager.isExist(mFontName))
			{
				manager.removeByName(mFontName);
			}

			MyGUI::xml::Document document;
			document.createDeclaration();
			MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
			generateFontTTFXml(root, mFontName);

			MyGUI::ResourceManager::getInstance().loadFromXmlNode(root, "", MyGUI::Version());
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

	void FontPanel::generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName)
	{
		_root->addAttribute("type", "Resource");
		_root->addAttribute("version", "1.1");

		MyGUI::xml::ElementPtr node = _root->createChild("Resource");
		node->addAttribute("type", "ResourceTrueTypeFont");
		node->addAttribute("name", _fontName);

		addProperty(node, "Source", mComboFont->getOnlyText());
		addProperty(node, "Size", MyGUI::utility::parseValue<int>(mEditSize->getOnlyText()));
		addProperty(node, "Resolution", MyGUI::utility::parseValue<int>(mEditResolution->getOnlyText()));
		addProperty(node, "Antialias", MyGUI::utility::parseValue<bool>(mComboAntialias->getOnlyText()));
		addProperty(node, "SpaceWidth", MyGUI::utility::parseValue<int>(mEditSpace->getOnlyText()));
		addProperty(node, "TabWidth", MyGUI::utility::parseValue<int>(mEditTab->getOnlyText()));
		addProperty(node, "CursorWidth", MyGUI::utility::parseValue<int>(mEditCursor->getOnlyText()));
		addProperty(node, "Distance", MyGUI::utility::parseValue<int>(mEditDistance->getOnlyText()));
		addProperty(node, "OffsetHeight", MyGUI::utility::parseValue<int>(mEditOffset->getOnlyText()));

		MyGUI::xml::ElementPtr node_codes = node->createChild("Codes");

		if (mEditRange1->getOnlyText() != "")
			node_codes->createChild("Code")->addAttribute("range", mEditRange1->getOnlyText());
		if (mEditRange2->getOnlyText() != "")
			node_codes->createChild("Code")->addAttribute("range", mEditRange2->getOnlyText());
		if (mEditHide->getOnlyText() != "")
			node_codes->createChild("Code")->addAttribute("hide", mEditHide->getOnlyText());
	}

	void FontPanel::generateFontManualXml(MyGUI::xml::ElementPtr _root, const std::string& _textureName, const std::string& _fontName)
	{
		_root->addAttribute("type", "Resource");
		_root->addAttribute("version", "1.1");

		MyGUI::xml::ElementPtr node = _root->createChild("Resource");
		node->addAttribute("type", "ResourceManualFont");
		node->addAttribute("name", _fontName);

		addProperty(node, "Source", _textureName);
		addProperty(node, "DefaultHeight", mFontHeight);

		MyGUI::IFont* font = MyGUI::FontManager::getInstance().getByName(mFontName);
		MyGUI::xml::Element* codes = node->createChild("Codes");

		addCode(codes, MyGUI::FontCodeType::Cursor, font);
		addCode(codes, MyGUI::FontCodeType::Selected, font);
		addCode(codes, MyGUI::FontCodeType::SelectedBack, font);

		addCode(codes, 32, font);
		addCode(codes, 9, font);

		MyGUI::IntSize range1 = MyGUI::IntSize::parse(mEditRange1->getOnlyText());
		MyGUI::IntSize range2 = MyGUI::IntSize::parse(mEditRange2->getOnlyText());
		MyGUI::IntSize hide1 = MyGUI::IntSize::parse(mEditHide->getOnlyText());

		for (int index = range1.width; index <= range1.height; ++ index)
		{
			if (index < hide1.width || index > hide1.height)
				addCode(codes, index, font);
		}

		for (int index = range2.width; index <= range2.height; ++ index)
		{
			if (index < hide1.width || index > hide1.height)
				addCode(codes, index, font);
		}
	}

	void FontPanel::addCode(MyGUI::xml::Element* _node, MyGUI::Char _code, MyGUI::IFont* _font)
	{
		MyGUI::GlyphInfo* info = _font->getGlyphInfo(_code);
		MyGUI::xml::Element* node = _node->createChild("Code");
		node->addAttribute("index", _code);
		node->addAttribute("coord", getCoord(info->uvRect, _font->getTextureFont()->getWidth(), _font->getTextureFont()->getHeight(), info->width, _font->getDefaultHeight()));
	}

	MyGUI::IntCoord FontPanel::getCoord(const MyGUI::FloatRect& _rect, int _textureWidth, int _textureHeight, int _charWidth, int _fontHeight)
	{
		int left = (int)((float)_rect.left * (float)_textureWidth);
		int top = (int)((float)_rect.top * (float)_textureHeight);

		return MyGUI::IntCoord(left, top, _charWidth, _fontHeight);
	}

} // namespace demo
