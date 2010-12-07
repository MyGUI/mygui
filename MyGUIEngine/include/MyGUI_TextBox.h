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
#ifndef __MYGUI_TEXT_BOX_H__
#define __MYGUI_TEXT_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class MYGUI_EXPORT TextBox :
		public Widget
	{
		MYGUI_RTTI_DERIVED( TextBox )

	public:
		TextBox();

		/** Get text region coordinate */
		virtual IntCoord getTextRegion();

		/** Get text region size */
		virtual IntSize getTextSize();

		/** Set widget caption */
		virtual void setCaption(const UString& _value);
		/** Get widget caption */
		virtual const UString& getCaption();

		/** Set widget text font */
		virtual void setFontName(const std::string& _value);
		/** Get widget text font name */
		const std::string& getFontName();

		/** Set widget text font height */
		virtual void setFontHeight(int _value);
		/** Get widget text font height */
		int getFontHeight();

		/** Set widget text align */
		virtual void setTextAlign(Align _value);
		/** Get widget text align */
		Align getTextAlign();

		/** Set widget text colour */
		virtual void setTextColour(const Colour& _value);
		/** Get widget text colour */
		const Colour& getTextColour();

		// устанавливает строку заменив /n на реальный перенос
		void setCaptionWithNewLine(const std::string& _value);

	protected:
		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);
	};

} // namespace MyGUI

#endif // __MYGUI_TEXT_BOX_H__
