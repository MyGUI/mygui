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

	class _MyGUIExport ControllerFadeAlpha : public ControllerItem
	{
	public:
		enum ControllerAction
		{
			ACTION_NONE,
			ACTION_HIDE,
			ACTION_DESTROY,
		};

	public:
		ControllerFadeAlpha(float _alpha, float _coef, ControllerAction _action, bool _enabled);

	private:
		ControllerFadeAlpha();

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);
		void replaseItem(WidgetPtr _widget, ControllerItem * _item);

		inline ControllerAction getAction() {return mAction;}
		inline bool getEnabled() {return mEnabled;}
		inline float getAlpha() {return mAlpha;}
		inline float getCoef() {return mCoef;}

	private:
		ControllerAction mAction;
		bool mEnabled;
		float mAlpha;
		float mCoef;

	};

}

#endif // __MYGUI_CONTROLLER_FADE_ALPHA_H__
