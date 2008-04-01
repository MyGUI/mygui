/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_FADE_ALPHA_H__
#define __MYGUI_CONTROLLER_FADE_ALPHA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	class _MyGUIExport ControllerPosition : public ControllerItem
	{
	public:
		ControllerPosition(float _speed);

	private:
		ControllerPosition();

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);
		void replaseItem(WidgetPtr _widget, ControllerItem * _item);

	private:
		float mRadius;
		float mAngle;
		float mSpeed;

		float mDeltaX;
		float mDeltaY;

	};

}

#endif // __MYGUI_CONTROLLER_FADE_ALPHA_H__
