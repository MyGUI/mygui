/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_IntSize.h"
#include "MMyGUI_IntCoord.h"

namespace MMyGUI
{

	public value struct SubWidgetText
	{
	internal:
		SubWidgetText(MyGUI::ISubWidgetText* _text) : mText(_text)
		{
		}

	public:
		IntSize GetTextSize()
		{
			if (mText == nullptr) return IntSize();
			MMYGUI_CHECK_NATIVE(mText);
			return mText->getTextSize();
		}

		IntCoord GetCoord()
		{
			if (mText == nullptr) return IntCoord();
			MMYGUI_CHECK_NATIVE(mText);
			return mText->getCoord();
		}

	private:
		MyGUI::ISubWidgetText* mText;
	};

	template <> struct Convert<MyGUI::ISubWidgetText*>
	{
		typedef SubWidgetText Type;
		inline static SubWidgetText To(MyGUI::ISubWidgetText* _value)
		{
			return SubWidgetText(_value);
		}
	};

} // namespace MMyGUI
