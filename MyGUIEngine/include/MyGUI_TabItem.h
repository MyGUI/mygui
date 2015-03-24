/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TAB_ITEM_H_
#define MYGUI_TAB_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TabControl.h"

namespace MyGUI
{

	/** \brief @wpage{TabItem}
		TabItem widget description should be here.
	*/
	class MYGUI_EXPORT TabItem :
        public TextBox // FIXME temporary use TextBox instead of Widget for caption
	{
		MYGUI_RTTI_DERIVED( TabItem )

	public:
		TabItem();

		/** Set item caption */
		virtual void setCaption(const UString& _value);
		/** Get item caption */
		virtual const UString& getCaption();

		//! Set button width
		void setButtonWidth(int _value = DEFAULT);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);
	};

} // namespace MyGUI

#endif // MYGUI_TAB_ITEM_H_
