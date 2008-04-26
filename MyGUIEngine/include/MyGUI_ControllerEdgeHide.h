/*!
	@file
	@author		Evmenov Georgiy
	@date		04/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_EDGE_HIDE_H__
#define __MYGUI_CONTROLLER_EDGE_HIDE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	class _MyGUIExport ControllerEdgeHide : public ControllerItem
	{
	public:
		/**
			@param _time in which widget will be hidden or shown
			@param _remainPixels how many pixels you will see afterr full hide
			@param _shadowSize adds to _remainPixels when hiding left or top (for example used for windows with shadows)
		*/
		ControllerEdgeHide(float _time, int _remainPixels = 0, int _shadowSize = 0);

	private:
		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);
		void replaseItem(WidgetPtr _widget, ControllerItem * _item);

		delegates::CDelegate1<WidgetPtr> eventPostAction;

		float mTime;
		int mRemainPixels;
		int mShadowSize;
		float mElapsedTime;
	};

}

#endif // __MYGUI_CONTROLLER_EDGE_HIDE_H__
