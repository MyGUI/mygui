/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_FADE_ALPHA_H__
#define __MYGUI_CONTROLLER_FADE_ALPHA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_ControllerInterface.h"

namespace MyGUI
{

	const float DEFAULT_SPEED_ALPHA = 1.0f;

	struct AlphaItem
	{
		AlphaItem(WidgetPtr _widget, float _alpha, float _coef, bool _hide, bool _destroy) :
			widget(_widget), alpha(_alpha), hide(_hide), destroy(_destroy), coef(_coef)
		{ }

		WidgetPtr widget;
		float alpha;
		bool hide;
		bool destroy;
		float coef;
	};

	typedef std::list<AlphaItem> ListAlphaItem;

	class _MyGUIExport ControllerFadeAlpha : public FrameListener, public ControllerInterface
	{
		INSTANCE_HEADER(ControllerFadeAlpha);

	public:
		void addItem(WidgetPtr _widget, float _alpha, float _coef = DEFAULT_SPEED_ALPHA, bool _hide = false, bool _enabled = true, bool _destroy = false);
		void removeItem(WidgetPtr _widget);
	
	private:
		void _frameEntered(float _time);
		void _unlinkWidget(WidgetPtr _widget);

	private:
		// список виджетов, для изменения альфы
		ListAlphaItem mListItem;
	};

}

#endif // __MYGUI_CONTROLLER_FADE_ALPHA_H__
