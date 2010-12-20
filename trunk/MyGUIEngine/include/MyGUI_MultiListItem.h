/*!
	@file
	@author		Albert Semenov
	@date		12/2010
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
#ifndef __MYGUI_MULTI_LIST_ITEM_H__
#define __MYGUI_MULTI_LIST_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TabControl.h"
#include "MyGUI_ResizingPolicy.h"

namespace MyGUI
{

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
