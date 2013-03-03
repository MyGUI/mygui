/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_CONTROLLER_EDGE_HIDE_H__
#define __MYGUI_CONTROLLER_EDGE_HIDE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	/** This controller used for hiding widgets near screen edges.
		Widget will start hiding(move out of screen) if it's near
		border and it and it's childrens don't have any focus. Hiding
		till only small part of widget be visible. Widget will move
		inside screen if it have any focus.
	*/
	class MYGUI_EXPORT ControllerEdgeHide :
		public ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerEdgeHide )

	public:
		ControllerEdgeHide();
		virtual ~ControllerEdgeHide();

		/**
			@param _value in which widget will be hidden or shown
		*/
		void setTime(float _value);

		/**
			@param _value How many pixels you will see after full hide
		*/
		void setRemainPixels(int _value);

		/**
			@param _value Added to "remain pixels" when hiding left or top (for example used for windows with shadows)
		*/
		void setShadowSize(int _value);

		virtual void setProperty(const std::string& _key, const std::string& _value);

	private:
		bool addTime(Widget* _widget, float _time);
		void prepareItem(Widget* _widget);

		void recalculateTime(Widget* _widget);

		delegates::CMultiDelegate1<Widget*> eventPostAction;

		float mTime;
		int mRemainPixels;
		int mShadowSize;
		float mElapsedTime;
		// for checking if widget was moved
		MyGUI::IntCoord mLastCoord;
	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_EDGE_HIDE_H__
