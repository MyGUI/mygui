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

	namespace
	{

		const char* const fileTypesData[] = { "*.ttf", "*.ttc", "*.otf", "*.pfa", "*.pfb", "*.fon" };

		const MyGUI::VectorString fileTypes(fileTypesData, fileTypesData + sizeof fileTypesData / sizeof *fileTypesData);

		struct StrCmpI : public std::binary_function<std::string, std::string, bool>
		{
			result_type operator()(const first_argument_type& _first, const second_argument_type& _second)
			{
				return _strcmpi(_first.c_str(), _second.c_str()) < 0;
			}
		};

	}

	FontPanel::FontPanel() :
		wraps::BaseLayout("FontPanel.layout"),
		mFontName("GeneratedFontName"),
		mFontHeight(0),
		mFontView(new FontView),
		mTextureView(new TextureView)
	{
		assignWidget(mComboFont, "combo_Fonts");
		assignWidget(mEditSize, "edit_Size");
		assignWidget(mComboResolution, "combo_Resolution");
		assignWidget(mComboAntialias, "combo_Antialias");
		assignWidget(mComboSpace, "combo_Space");
		assignWidget(mComboTab, "combo_Tab");
		assignWidget(mComboOffset, "combo_Offset");
		assignWidget(mEditRange1A, "edit_Range1A");
		assignWidget(mEditRange1B, "edit_Range1B");
		assignWidget(mEditRange2A, "edit_Range2A");
		assignWidget(mEditRange2B, "edit_Range2B");
		assignWidget(mEditHideA, "edit_HideA");
		assignWidget(mEditHideB, "edit_HideB");
		assignWidget(mComboSubstituteCode, "combo_SubstituteCode");
		assignWidget(mButtonGenerate, "button_Generate");
		assignWidget(mEditSaveFileName, "edit_SaveFileName");
		assignWidget(mButtonSave, "button_Save");

		mComboAntialias->setIndexSelected(0);

		initializeComboBox(mComboResolution);
		initializeComboBox(mComboSpace);
		initializeComboBox(mComboTab);
		initializeComboBox(mComboOffset);
		initializeComboBox(mComboSubstituteCode);

		mButtonGenerate->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);
		mButtonSave->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

		update();
	}

	FontPanel::~FontPanel()
	{
		delete mFontView;
		delete mTextureView;
	}

	void FontPanel::initializeComboBox(MyGUI::ComboBox* comboBox)
	{
		if (comboBox != nullptr)
		{
			comboBox->setIndexSelected(0);

			comboBox->eventKeySetFocus += MyGUI::newDelegate(this, &FontPanel::notifyComboBoxKeySetFocus);
			comboBox->eventKeyLostFocus += MyGUI::newDelegate(this, &FontPanel::notifyComboBoxKeyLostFocus);
		}
	}

	void FontPanel::update()
	{
		mComboFont->removeAllItems();

		std::set<std::string, StrCmpI> allFilenames;

		for (MyGUI::VectorString::const_iterator fileType = fileTypes.begin(); fileType != fileTypes.end(); ++fileType)
		{
			const MyGUI::VectorString& filenames = MyGUI::DataManager::getInstance().getDataListNames(*fileType);
			allFilenames.insert(filenames.begin(), filenames.end());
		}

		for (std::set<std::string, StrCmpI>::const_iterator iter = allFilenames.begin(); iter != allFilenames.end(); ++iter)
			mComboFont->addItem(*iter);

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
			MyGUI::Message::createMessageBox("Message", "Success", _fileName, MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "Error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::saveFontManualXml(const std::string& _fontName, const std::string& _textureName, const std::string& _fileName)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		generateFontManualXml(root, _textureName, _fontName);

		if (document.save(_fileName))
			MyGUI::Message::createMessageBox("Message", "Success", _fileName, MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "Error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::notifyComboBoxKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		MyGUI::ComboBox* comboBox = static_cast<MyGUI::ComboBox*>(_sender);

		if (comboBox->getIndexSelected() != MyGUI::ITEM_NONE)
			comboBox->setCaption(MyGUI::UString());
	}

	void FontPanel::notifyComboBoxKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		MyGUI::ComboBox* comboBox = static_cast<MyGUI::ComboBox*>(_sender);

		if (comboBox->getCaption().empty())
			comboBox->setIndexSelected(0);
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

			try
			{
				MyGUI::ResourceManager::getInstance().loadFromXmlNode(root, "", MyGUI::Version());

				MyGUI::IResource* resource = manager.getByName(mFontName, false);
				MYGUI_ASSERT(resource != nullptr, "Could not find font '" << mFontName << "'");
				MyGUI::IFont* font = resource->castType<MyGUI::IFont>();

				// вывод реального размера шрифта
				mFontHeight = font->getDefaultHeight();

				mFontView->setFontName(mFontName);
				mTextureView->setFontName(mFontName);
			}
			catch (MyGUI::Exception & e)
			{
				mFontHeight = 0;

				mFontView->setFontName(MyGUI::FontManager::getInstance().getDefaultFont());
				mTextureView->setFontName("");

				MyGUI::Message::createMessageBox("Message", "Error", e.getDescription(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
			}
		}
	}

	void addCodeRange(MyGUI::EditBox* a, MyGUI::EditBox* b, MyGUI::xml::ElementPtr node_codes, const std::string& attribute)
	{
		MyGUI::uint32 range1a = MyGUI::utility::parseValue<MyGUI::uint32>(a->getOnlyText());
		MyGUI::uint32 range1b = MyGUI::utility::parseValue<MyGUI::uint32>(b->getOnlyText());

		if (a->getTextLength() != 0)
		{
			std::string range = (b->getTextLength() != 0) ? MyGUI::utility::toString(a->getOnlyText(), " ", b->getOnlyText()) : a->getOnlyText();

			node_codes->createChild("Code")->addAttribute(attribute, range);
		}
		else if (b->getTextLength() != 0)
		{
			node_codes->createChild("Code")->addAttribute(attribute, b->getOnlyText());
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
		addProperty(node, "Size", MyGUI::utility::parseValue<float>(mEditSize->getOnlyText()));

		if (mComboResolution->getIndexSelected() != 0)
			addProperty(node, "Resolution", MyGUI::utility::parseValue<int>(mComboResolution->getOnlyText()));

		if (mComboAntialias->getIndexSelected() != 0)
			addProperty(node, "Antialias", MyGUI::utility::parseValue<bool>(mComboAntialias->getOnlyText()));

		if (mComboSpace->getIndexSelected() != 0)
			addProperty(node, "SpaceWidth", MyGUI::utility::parseValue<int>(mComboSpace->getOnlyText()));

		if (mComboTab->getIndexSelected() != 0)
			addProperty(node, "TabWidth", MyGUI::utility::parseValue<int>(mComboTab->getOnlyText()));

		if (mComboOffset->getIndexSelected() != 0)
			addProperty(node, "OffsetHeight", MyGUI::utility::parseValue<int>(mComboOffset->getOnlyText()));

		MyGUI::xml::ElementPtr node_codes = node->createChild("Codes");

		addCodeRange(mEditRange1A, mEditRange1B, node_codes, "range");
		addCodeRange(mEditRange2A, mEditRange2B, node_codes, "range");
		addCodeRange(mEditHideA, mEditHideB, node_codes, "hide");

		if (!node_codes->getElementEnumerator().next())
			node->removeChild(node_codes);

		if (mComboSubstituteCode->getIndexSelected() != 0)
			addProperty(node, "SubstituteCode", MyGUI::utility::parseValue<int>(mComboSubstituteCode->getOnlyText()));
	}

	void FontPanel::generateFontManualXml(MyGUI::xml::ElementPtr _root, const std::string& _textureName, const std::string& _fontName)
	{
		MyGUI::ResourceTrueTypeFont* font = dynamic_cast<MyGUI::ResourceTrueTypeFont*>(MyGUI::FontManager::getInstance().getByName(mFontName));

		if (font != nullptr)
		{
			_root->addAttribute("type", "Resource");
			_root->addAttribute("version", "1.1");

			MyGUI::xml::ElementPtr node = _root->createChild("Resource");
			node->addAttribute("type", "ResourceManualFont");
			node->addAttribute("name", _fontName);

			addProperty(node, "Source", _textureName);
			addProperty(node, "DefaultHeight", mFontHeight);

			MyGUI::xml::Element* codes = node->createChild("Codes");

			std::vector<std::pair<MyGUI::Char, MyGUI::Char> > codePointRanges = font->getCodePointRanges();
			MyGUI::Char substituteCodePoint = font->getSubstituteCodePoint();
			bool isCustomSubstituteCodePoint = substituteCodePoint != MyGUI::FontCodeType::NotDefined;

			// Add all of the code points. Skip over the substitute code point -- unless it's been customized, in which case it
			// needs to be added here as a regular code point and then at the end as a substitute code point.
			for (std::vector<std::pair<MyGUI::Char, MyGUI::Char> >::const_iterator iter = codePointRanges.begin() ; iter != codePointRanges.end(); ++iter)
				for (MyGUI::Char code = iter->first; code <= iter->second && code >= iter->first; ++code)
					if (code != substituteCodePoint || isCustomSubstituteCodePoint)
						addCode(codes, code, font, false);

			// Always add the substitute code point last, even if it isn't the last one in the range.
			addCode(codes, substituteCodePoint, font, true);
		}
	}

	void FontPanel::addCode(MyGUI::xml::Element* _node, MyGUI::Char _code, MyGUI::ResourceTrueTypeFont* _font, bool _isSubstitute)
	{
		MyGUI::xml::Element* codeNode = _node->createChild("Code");

		if (!_isSubstitute)
		{
			switch (_code)
			{
			case MyGUI::FontCodeType::Selected:
				codeNode->addAttribute("index", "selected");
				break;

			case MyGUI::FontCodeType::SelectedBack:
				codeNode->addAttribute("index", "selected_back");
				break;

			case MyGUI::FontCodeType::Cursor:
				codeNode->addAttribute("index", "cursor");
				break;

			default:
				codeNode->addAttribute("index", _code);
				break;
			}
		}
		else
		{
			codeNode->addAttribute("index", "substitute");
		}

		MyGUI::GlyphInfo* info = _font->getGlyphInfo(_code);
		MyGUI::ITexture* texture = _font->getTextureFont();
		MyGUI::FloatCoord coord(info->uvRect.left * (float)texture->getWidth(), info->uvRect.top * (float)texture->getHeight(), info->width, info->height);

		if (!coord.empty())
			codeNode->addAttribute("coord", coord);

		if (info->bearingX != 0.0f || info->bearingY != 0.0f)
			codeNode->addAttribute("bearing", MyGUI::FloatPoint(info->bearingX, info->bearingY));

		if (info->advance != info->width)
			codeNode->addAttribute("advance", info->advance);
	}

} // namespace demo
