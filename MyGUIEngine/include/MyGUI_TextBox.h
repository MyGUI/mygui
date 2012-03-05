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

	/** \brief @wpage{TextBox}
		TextBox widget description should be here.
	*/
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

		/** Set TextBox caption and replace special sequences.\n
			"\\n" will be replaced with new line character;\n
			"#{Keyword}" replaced with string from LanguageManager or
			left as it is if kayword wasn't found.
		*/
		void setCaptionWithReplacing(const std::string& _value);

		/** Set widget text shadow colour */
		virtual void setTextShadowColour(const Colour& _value);
		/** Get widget text shadow colour */
		const Colour& getTextShadowColour();

		/** Set widget text shadow */
		virtual void setTextShadow(bool _value);
		/** Get widget text shadow */
		bool getTextShadow();

	protected:
		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);
	};

} // namespace MyGUI

#endif // __MYGUI_TEXT_BOX_H__
