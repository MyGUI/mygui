/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MULTI_LIST_ITEM_H_
#define MYGUI_MULTI_LIST_ITEM_H_

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
		void setCaption(const UString& _value) override;
		/** Get item caption */
		const UString& getCaption() override;

		void setItemResizingPolicy(ResizingPolicy _value);

		void setItemWidth(int _value);

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(const std::string& _key, const std::string& _value) override;

	private:
		MultiListBox* getOwner();

	};

} // namespace MyGUI

#endif // MYGUI_MULTI_LIST_ITEM_H_
