/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
		EventPair<EventHandle_WidgetSizeT, EventHandle_VScrollPtrSizeT> eventScrollChangePosition;
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
