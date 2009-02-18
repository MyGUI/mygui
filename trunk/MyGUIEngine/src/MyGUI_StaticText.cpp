/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*//*
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

namespace MyGUI
{

	StaticText::StaticText(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		initialiseWidgetSkin(_info);
	}

	StaticText::~StaticText()
	{
		shutdownWidgetSkin();
	}

	void StaticText::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void StaticText::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// ������ ��������
		const MapString & properties = _info->getProperties();
		if (false == properties.empty())
		{
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("FontName")) != properties.end()) setFontName(iter->second);
			if ((iter = properties.find("FontHeight")) != properties.end()) setFontHeight(utility::parseUInt(iter->second));
			if ((iter = properties.find("TextAlign")) != properties.end()) setTextAlign(Align::parse(iter->second));
			if ((iter = properties.find("TextColour")) != properties.end()) setTextColour(Colour::parse(iter->second));
		}
	}

	void StaticText::shutdownWidgetSkin()
	{
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

	void StaticText::setFontName(const Ogre::String & _font)
	{
		if (nullptr != mText) mText->setFontName(_font);
	}

	const std::string & StaticText::getFontName()
	{
		if (nullptr == mText) {
			static std::string empty;
			return empty;
		}
		return mText->getFontName();
	}

	void StaticText::setFontHeight(uint _height)
	{
		if (nullptr != mText) mText->setFontHeight(_height);
	}

	uint StaticText::getFontHeight()
	{
		return (nullptr == mText) ? 0 : mText->getFontHeight();
	}

} // namespace MyGUI
