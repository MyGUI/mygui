/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_MULTI_LIST_ITEM_H__
#define __MYGUI_MULTI_LIST_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TabControl.h"
#include "MyGUI_ResizingPolicy.h"

namespace MyGUI
{

	/** \brief @wpage{MultiListItem}
		MultiListItem widget description should be here.
	*/
	class MYGUI_EXPORT MultiListItem :
		public TextBox
	{
		MYGUI_RTTI_DERIVED( MultiListItem )

	public:
		MultiListItem();

		/** Set item caption */
		virtual void setCaption(const UString& _value);
		/** Get item caption */
		virtual const UString& getCaption();

		void setItemResizingPolicy(ResizingPolicy _value);

		void setItemWidth(int _value);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		MultiListBox* getOwner();

	};

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_ITEM_H__
