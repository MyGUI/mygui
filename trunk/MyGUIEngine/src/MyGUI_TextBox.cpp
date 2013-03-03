/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	TextBox::TextBox()
	{
	}

	IntCoord TextBox::getTextRegion()
	{
		return (nullptr == getSubWidgetText()) ? IntCoord() : getSubWidgetText()->getCoord();
	}

	IntSize TextBox::getTextSize()
	{
		return (nullptr == getSubWidgetText()) ? IntSize() : getSubWidgetText()->getTextSize();
	}

	void TextBox::setTextAlign(Align _value)
	{
		if (getSubWidgetText() != nullptr)
			getSubWidgetText()->setTextAlign(_value);
	}

	Align TextBox::getTextAlign()
	{
		if (getSubWidgetText() != nullptr)
			return getSubWidgetText()->getTextAlign();
		return Align::Default;
	}

	void TextBox::setTextColour(const Colour& _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setTextColour(_value);
	}

	const Colour& TextBox::getTextColour()
	{
		return (nullptr == getSubWidgetText()) ? Colour::Zero : getSubWidgetText()->getTextColour();
	}

	void TextBox::setFontName(const std::string& _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setFontName(_value);
	}

	const std::string& TextBox::getFontName()
	{
		if (nullptr == getSubWidgetText())
			return Constants::getEmptyString();
		return getSubWidgetText()->getFontName();
	}

	void TextBox::setFontHeight(int _height)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setFontHeight(_height);
	}

	int TextBox::getFontHeight()
	{
		return (nullptr == getSubWidgetText()) ? 0 : getSubWidgetText()->getFontHeight();
	}

	void TextBox::setCaption(const UString& _caption)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setCaption(_caption);
	}

	const UString& TextBox::getCaption()
	{
		if (nullptr == getSubWidgetText())
			return Constants::getEmptyUString();
		return getSubWidgetText()->getCaption();
	}

	void TextBox::setCaptionWithReplacing(const std::string& _value)
	{
		// replace "\\n" with char '\n'
		size_t pos = _value.find("\\n");
		if (pos == std::string::npos)
		{
			setCaption(LanguageManager::getInstance().replaceTags(_value));
		}
		else
		{
			std::string value(_value);
			while (pos != std::string::npos)
			{
				value[pos++] = '\n';
				value.erase(pos, 1);
				pos = value.find("\\n");
			}
			setCaption(LanguageManager::getInstance().replaceTags(value));
		}
	}

	void TextBox::setTextShadowColour(const Colour& _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setShadowColour(_value);
	}

	const Colour& TextBox::getTextShadowColour()
	{
		return (nullptr == getSubWidgetText()) ? Colour::Black : getSubWidgetText()->getShadowColour();
	}

	void TextBox::setTextShadow(bool _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setShadow(_value);
	}

	bool TextBox::getTextShadow()
	{
		return (nullptr == getSubWidgetText()) ? false : getSubWidgetText()->getShadow();
	}

	void TextBox::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{TextBox, TextColour, Colour} Цвет текста.
		if (_key == "TextColour")
			setTextColour(utility::parseValue<Colour>(_value));

		/// @wproperty{TextBox, TextAlign, Align} Выравнивание текста.
		else if (_key == "TextAlign")
			setTextAlign(utility::parseValue<Align>(_value));

		/// @wproperty{TextBox, FontName, string} Имя шрифта.
		else if (_key == "FontName")
			setFontName(_value);

		/// @wproperty{TextBox, FontHeight, int} Высота шрифта.
		else if (_key == "FontHeight")
			setFontHeight(utility::parseValue<int>(_value));

		/// @wproperty{TextBox, Caption, string} Содержимое поля редактирования.
		else if (_key == "Caption")
			setCaptionWithReplacing(_value);

		/// @wproperty{TextBox, TextShadowColour, Colour} Цвет тени текста.
		else if (_key == "TextShadowColour")
			setTextShadowColour(utility::parseValue<Colour>(_value));

		/// @wproperty{TextBox, TextShadow, bool} Режим показа тени текста.
		else if (_key == "TextShadow")
			setTextShadow(utility::parseValue<bool>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
