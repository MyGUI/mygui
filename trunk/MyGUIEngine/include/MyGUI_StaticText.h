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
#ifndef __MYGUI_STATIC_TEXT_H__
#define __MYGUI_STATIC_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class MYGUI_EXPORT StaticText : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<StaticText>;

		MYGUI_RTTI_CHILD_HEADER( StaticText, Widget );

		/** Get text region coordinate */
		IntCoord getTextRegion();

		/** Get text region size */
		IntSize getTextSize();

		/** Set widget text font */
		virtual void setFontName(const std::string & _font);
		/** Get widget text font name */
		const std::string & getFontName();

		/** Set widget text font height */
		virtual void setFontHeight(uint _height);
		/** Get widget text font height */
		uint getFontHeight();

		/** Set widget text align */
		virtual void setTextAlign(Align _align);
		/** Get widget text align */
		Align getTextAlign();

		/** Set widget text colour */
		virtual void setTextColour(const Colour& _colour);
		/** Get widget text colour */
		const Colour& getTextColour();

	protected:
		StaticText(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~StaticText();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	};

} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_H__
