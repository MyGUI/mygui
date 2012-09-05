/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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
#ifndef __MYGUI_TAB_ITEM_H__
#define __MYGUI_TAB_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TabControl.h"

namespace MyGUI
{

	/** \brief @wpage{TabItem}
		TabItem widget description should be here.
	*/
	class MYGUI_EXPORT TabItem :
		public TextBox // FIXME пока для кэпшена вместо виджета текст (Bug #190)
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

#endif // __MYGUI_TAB_ITEM_H__
