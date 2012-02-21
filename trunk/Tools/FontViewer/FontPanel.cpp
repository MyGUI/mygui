/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include <cctype>
#include "FontPanel.h"
#include "MessageBox/MessageBox.h"

namespace demo
{

	namespace
	{

		const char* const fileTypesData[] = { "*.ttf", "*.ttc", "*.otf", "*.pfa", "*.pfb", "*.fon", "*.fnt" };

		const MyGUI::VectorString fileTypes(fileTypesData, fileTypesData + sizeof fileTypesData / sizeof *fileTypesData);

		struct StrCmpI : public std::binary_function<std::string, std::string, bool>
		{
			result_type operator()(const first_argument_type& _a, const second_argument_type& _b)
			{
				size_t aLength = _a.length(), bLength = _b.length(), length = std::min(aLength, bLength);
				first_argument_type::const_iterator aIter = _a.begin();
				second_argument_type::const_iterator bIter = _b.begin();

				while (length-- > 0)
				{
					int aUpper = toupper(*aIter++), bUpper = toupper(*bIter++);

					if (aUpper != bUpper)
						return aUpper < bUpper;
				}

				return aLength < bLength;
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

		initializeComboBox(mComboFont);
		initializeEditBox(mEditSize);
		initializeComboBox(mComboResolution);
		initializeComboBox(mComboAntialias);
		initializeComboBox(mComboSpace);
		initializeComboBox(mComboTab);
		initializeComboBox(mComboOffset);
		initializeEditBox(mEditRange1A);
		initializeEditBox(mEditRange1B);
		initializeEditBox(mEditRange2A);
		initializeEditBox(mEditRange2B);
		initializeEditBox(mEditHideA);
		initializeEditBox(mEditHideB);
		initializeComboBox(mComboSubstituteCode);

		mButtonGenerate->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);
		mButtonSave->eventMouseButtonClick += MyGUI::newDelegate(this, &FontPanel::notifyMouseButtonClick);

		enableSave(false);

		update();
	}

	FontPanel::~FontPanel()
	{
		delete mFontView;
		delete mTextureView;
	}

	void FontPanel::initializeEditBox(MyGUI::EditBox* _editBox)
	{
		if (_editBox != nullptr)
			_editBox->eventEditTextChange += MyGUI::newDelegate(this, &FontPanel::notifyTextChange);
	}

	void FontPanel::initializeComboBox(MyGUI::ComboBox* _comboBox)
	{
		if (_comboBox != nullptr)
		{
			if (_comboBox->getItemCount() > 0)
				_comboBox->setIndexSelected(0);

			_comboBox->eventComboChangePosition += MyGUI::newDelegate(this, &FontPanel::notifyComboBoxChangePosition);

			if (!_comboBox->getComboModeDrop())
			{
				initializeEditBox(_comboBox);

				_comboBox->eventKeySetFocus += MyGUI::newDelegate(this, &FontPanel::notifyComboBoxKeySetFocus);
				_comboBox->eventKeyLostFocus += MyGUI::newDelegate(this, &FontPanel::notifyComboBoxKeyLostFocus);
			}
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

	void FontPanel::enableSave(bool _enable, const MyGUI::UString& fileName /*= MyGUI::UString()*/)
	{
		mEditSaveFileName->setCaption(fileName);
		mEditSaveFileName->setEnabled(_enable);
		mButtonSave->setEnabled(_enable);
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
		generateFontTTFXml(document.createRoot("MyGUI"), _fontName);

		if (document.save(_fileName))
			MyGUI::Message::createMessageBox("Message", "Success", "Saved font definition file: " + _fileName + "'", MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "Error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::saveFontManualXml(const std::string& _fontName, const std::string& _textureName, const std::string& _fileName)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		generateFontManualXml(document.createRoot("MyGUI"), _textureName, _fontName);

		if (document.save(_fileName))
			MyGUI::Message::createMessageBox("Message", "Success", "Saved manual font definition file: '" + _fileName + "'", MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
		else
			MyGUI::Message::createMessageBox("Message", "Error", document.getLastError(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
	}

	void FontPanel::notifyTextChange(MyGUI::EditBox* _sender)
	{
		enableSave(false);
	}

	void FontPanel::notifyComboBoxChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		enableSave(false);
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

			std::string fontName = mEditSaveFileName->getOnlyText();
			std::string fileName = fontName + ".xml";
			saveFontTTFXml(fontName, fileName);
			removeFont(fontName);

			fontName = MyGUI::utility::toString(mEditSaveFileName->getOnlyText(), ".manual");
			fileName = fontName + ".xml";
			saveFontManualXml(fontName, textureName, fileName);
			removeFont(fontName);
		}
		else if (_widget == mButtonGenerate)
		{
			removeFont(mFontName);

			MyGUI::xml::Document document;
			document.createDeclaration();
			MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
			generateFontTTFXml(root, mFontName);

			try
			{
				MyGUI::ResourceManager::getInstance().loadFromXmlNode(root, "", MyGUI::Version());

				MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().getByName(mFontName, false);
				MYGUI_ASSERT(resource != nullptr, "Could not find font '" << mFontName << "'");
				MyGUI::IFont* font = resource->castType<MyGUI::IFont>();

				// вывод реального размера шрифта
				mFontHeight = font->getDefaultHeight();

				mFontView->setFontName(mFontName);
				mTextureView->setFontName(mFontName);

				MyGUI::UString fileName = mComboFont->getOnlyText();

				MyGUI::UString::size_type dotIndex = fileName.find_last_of(".");

				if (dotIndex != MyGUI::UString::npos)
					fileName.erase(fileName.begin() + dotIndex, fileName.end());

				enableSave(true, MyGUI::utility::toString(fileName, '.', mFontHeight));
			}
			catch (MyGUI::Exception & e)
			{
				mFontHeight = 0;

				mFontView->setFontName(MyGUI::FontManager::getInstance().getDefaultFont());
				mTextureView->setFontName(std::string());

				enableSave(false);

				MyGUI::Message::createMessageBox("Message", "Error", e.getDescription(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
			}
		}
	}

	void addCodeRange(MyGUI::EditBox* a, MyGUI::EditBox* b, MyGUI::xml::ElementPtr node_codes, const std::string& attribute)
	{
		if (a->getTextLength() != 0)
		{
			std::string range = (b->getTextLength() != 0) ? MyGUI::utility::toString(a->getOnlyText(), " ", b->getOnlyText()) : std::string(a->getOnlyText());

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

	void FontPanel::removeFont(const std::string& _fontName)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
			if (manager.isExist(_fontName))
				manager.removeByName(_fontName);
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
