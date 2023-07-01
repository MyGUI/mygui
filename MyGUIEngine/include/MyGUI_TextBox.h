/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXT_BOX_H_
#define MYGUI_TEXT_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	/** \brief @wpage{TextBox}
		TextBox widget description should be here.
	*/
	class MYGUI_EXPORT TextBox : public Widget
	{
		MYGUI_RTTI_DERIVED(TextBox)

	public:
		/** Get text region coordinate */
		virtual IntCoord getTextRegion() const;

		/** Get text region size */
		virtual IntSize getTextSize() const;

		/** Set widget caption */
		virtual void setCaption(const UString& _caption);
		/** Get widget caption */
		virtual const UString& getCaption() const;

		/** Set widget text font */
		virtual void setFontName(std::string_view _value);
		/** Get widget text font name */
		std::string_view getFontName() const;

		/** Set widget text font height. If set to 0 then default height from selected font is used */
		virtual void setFontHeight(int _height);
		/** Get widget text font height */
		virtual int getFontHeight() const;

		/** Set widget text align */
		virtual void setTextAlign(Align _value);
		/** Get widget text align */
		Align getTextAlign() const;

		/** Set widget text colour */
		virtual void setTextColour(const Colour& _value);
		/** Get widget text colour */
		const Colour& getTextColour() const;

		/** Set TextBox caption and replace special sequences.\n
			"\\n" will be replaced with new line character;\n
			"#{Keyword}" replaced with string from LanguageManager or
			left as it is if kayword wasn't found.
		*/
		void setCaptionWithReplacing(std::string_view _value);

		/** Set widget text shadow colour */
		virtual void setTextShadowColour(const Colour& _value);
		/** Get widget text shadow colour */
		const Colour& getTextShadowColour() const;

		/** Set widget text shadow */
		virtual void setTextShadow(bool _value);
		/** Get widget text shadow */
		bool getTextShadow() const;

	protected:
		void setPropertyOverride(std::string_view _key, std::string_view _value) override;
	};

} // namespace MyGUI

#endif // MYGUI_TEXT_BOX_H_
