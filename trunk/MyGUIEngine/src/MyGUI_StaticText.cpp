/*!
	@file
	@author		Albert Semenov
	@date		12/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_LanguageManager.h"

namespace MyGUI
{

	StaticText::StaticText()
	{
	}

	void StaticText::initialiseWidgetSkin(ResourceSkin* _info)
	{
		Base::initialiseWidgetSkin(_info);

		// ������ ��������
		const MapString& properties = _info->getProperties();
		if (!properties.empty())
		{
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("FontName")) != properties.end()) setFontName(iter->second);
			if ((iter = properties.find("FontHeight")) != properties.end()) setFontHeight(utility::parseInt(iter->second));
			if ((iter = properties.find("TextAlign")) != properties.end()) setTextAlign(Align::parse(iter->second));
			if ((iter = properties.find("TextColour")) != properties.end()) setTextColour(Colour::parse(iter->second));
		}
	}

	void StaticText::shutdownWidgetSkin()
	{
		Base::shutdownWidgetSkin();
	}

	IntCoord StaticText::getTextRegion()
	{
		return (nullptr == mText) ? IntCoord() : mText->getCoord();
	}

	IntSize StaticText::getTextSize()
	{
		return (nullptr == mText) ? IntSize() : mText->getTextSize();
	}

	void StaticText::setTextAlign(Align _align)
	{
		if (mText != nullptr) mText->setTextAlign(_align);
	}

	Align StaticText::getTextAlign()
	{
		if (mText != nullptr) return mText->getTextAlign();
		return Align::Default;
	}

	void StaticText::setTextColour(const Colour& _colour)
	{
		if (nullptr != mText) mText->setTextColour(_colour);
	}

	const Colour& StaticText::getTextColour()
	{
		return (nullptr == mText) ? Colour::Zero : mText->getTextColour();
	}

	void StaticText::setFontName(const std::string& _font)
	{
		if (nullptr != mText) mText->setFontName(_font);
	}

	const std::string& StaticText::getFontName()
	{
		if (nullptr == mText)
		{
			static std::string empty;
			return empty;
		}
		return mText->getFontName();
	}

	void StaticText::setFontHeight(int _height)
	{
		if (nullptr != mText) mText->setFontHeight(_height);
	}

	int StaticText::getFontHeight()
	{
		return (nullptr == mText) ? 0 : mText->getFontHeight();
	}

	void StaticText::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Text_TextColour") setTextColour(utility::parseValue<Colour>(_value));
		else if (_key == "Text_TextAlign") setTextAlign(utility::parseValue<Align>(_value));
		else if (_key == "Text_FontName") setFontName(_value);
		else if (_key == "Text_FontHeight") setFontHeight(utility::parseValue<int>(_value));
		else if (_key == "Text_Caption") setCaptionWithNewLine(_value);
		else if (_key == "Widget_Caption") setCaptionWithNewLine(_value);// FIXME ������ �����
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	void StaticText::setCaption(const UString& _caption)
	{
		if (nullptr != mText)
			mText->setCaption(_caption);
	}

	const UString& StaticText::getCaption()
	{
		if (nullptr == mText)
		{
			// FIXME ������� ���� ������ ������
			static UString empty;
			return empty;
		}
		return mText->getCaption();
	}

	void StaticText::setCaptionWithNewLine(const std::string& _value)
	{
		// change '\n' on char 10
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

} // namespace MyGUI
