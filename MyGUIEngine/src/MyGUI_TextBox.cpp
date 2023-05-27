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

	IntCoord TextBox::getTextRegion() const
	{
		return (nullptr == getSubWidgetText()) ? IntCoord() : getSubWidgetText()->getCoord();
	}

	IntSize TextBox::getTextSize() const
	{
		return (nullptr == getSubWidgetText()) ? IntSize() : getSubWidgetText()->getTextSize();
	}

	void TextBox::setTextAlign(Align _value)
	{
		if (getSubWidgetText() != nullptr)
			getSubWidgetText()->setTextAlign(_value);
	}

	Align TextBox::getTextAlign() const
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

	const Colour& TextBox::getTextColour() const
	{
		return (nullptr == getSubWidgetText()) ? Colour::Zero : getSubWidgetText()->getTextColour();
	}

	void TextBox::setFontName(std::string_view _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setFontName(_value);
	}

	std::string_view TextBox::getFontName() const
	{
		if (nullptr == getSubWidgetText())
			return {};
		return getSubWidgetText()->getFontName();
	}

	void TextBox::setFontHeight(int _height)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setFontHeight(_height);
	}

	int TextBox::getFontHeight() const
	{
		return (nullptr == getSubWidgetText()) ? 0 : getSubWidgetText()->getFontHeight();
	}

	void TextBox::setCaption(const UString& _caption)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setCaption(_caption);
	}

	const UString& TextBox::getCaption() const
	{
		if (nullptr == getSubWidgetText())
			return Constants::getEmptyUString();
		return getSubWidgetText()->getCaption();
	}

	void TextBox::setCaptionWithReplacing(std::string_view _value)
	{
		// replace "\\n" with char '\n'
		size_t pos = _value.find("\\n");
		if (pos == std::string::npos)
		{
			setCaption(LanguageManager::getInstance().replaceTags(UString(_value)));
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

	const Colour& TextBox::getTextShadowColour() const
	{
		return (nullptr == getSubWidgetText()) ? Colour::Black : getSubWidgetText()->getShadowColour();
	}

	void TextBox::setTextShadow(bool _value)
	{
		if (nullptr != getSubWidgetText())
			getSubWidgetText()->setShadow(_value);
	}

	bool TextBox::getTextShadow() const
	{
		return (nullptr == getSubWidgetText()) ? false : getSubWidgetText()->getShadow();
	}

	void TextBox::setPropertyOverride(std::string_view _key, std::string_view _value)
	{
		/// @wproperty{TextBox, TextColour, Colour} Text colour.
		if (_key == "TextColour")
			setTextColour(utility::parseValue<Colour>(_value));

		/// @wproperty{TextBox, TextAlign, Align} Text alignment.
		else if (_key == "TextAlign")
			setTextAlign(utility::parseValue<Align>(_value));

		/// @wproperty{TextBox, FontName, string} Font name.
		else if (_key == "FontName")
			setFontName(_value);

		/// @wproperty{TextBox, FontHeight, int} Font height.
		else if (_key == "FontHeight")
			setFontHeight(utility::parseValue<int>(_value));

		/// @wproperty{TextBox, Caption, string} Text value.
		else if (_key == "Caption")
			setCaptionWithReplacing(_value);

		/// @wproperty{TextBox, TextShadowColour, Colour} Text shadow colour.
		else if (_key == "TextShadowColour")
			setTextShadowColour(utility::parseValue<Colour>(_value));

		/// @wproperty{TextBox, TextShadow, bool} Is text shadow visible.
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
