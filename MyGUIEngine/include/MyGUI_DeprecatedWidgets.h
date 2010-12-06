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
#ifndef __MYGUI_DEPRECATED_WIDGETS_H__
#define __MYGUI_DEPRECATED_WIDGETS_H__

#ifndef MYGUI_DONT_USE_OBSOLETE

#include "MyGUI_DeprecatedTypes.h"
#include "MyGUI_ScrollBar.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<VScroll*, size_t> EventHandle_VScrollPtrSizeT;

	class MYGUI_EXPORT VScroll :
		public ScrollBar
	{
		MYGUI_RTTI_DERIVED( VScroll )

	public:
		VScroll()
		{
			mVerticalAlignment = true;
		}

		/*events:*/
		/** Event : scroll tracker position changed.\n
			signature : void method(MyGUI::VScroll* _sender, size_t _position)\n
			@param _sender widget that called this event
			@param _position - new tracker position
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_VScrollPtrSizeT>
			eventScrollChangePosition;
	};

	class MYGUI_EXPORT HScroll :
		public VScroll
	{
		MYGUI_RTTI_DERIVED( HScroll )

	public:
		HScroll()
		{
			mVerticalAlignment = false;
		}
	};

} // namespace MyGUI

#endif // MYGUI_DONT_USE_OBSOLETE

#endif // __MYGUI_DEPRECATED_WIDGETS_H__
