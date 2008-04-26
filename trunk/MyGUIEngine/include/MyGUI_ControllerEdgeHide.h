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
			@param _time in which widget will reach destination coordinate
		*/
		ControllerEdgeHide(float _time);

	private:
		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);
		void replaseItem(WidgetPtr _widget, ControllerItem * _item);

		delegates::CDelegate1<WidgetPtr> eventPostAction;

		IntCoord mStartRect;
		IntCoord mDestRect;
		float mTime;
		float mElapsedTime;
	};

}

#endif // __MYGUI_CONTROLLER_EDGE_HIDE_H__
