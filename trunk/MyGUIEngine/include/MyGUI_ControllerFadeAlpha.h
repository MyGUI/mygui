/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_CONTROLLER_FADE_ALPHA_H__
#define __MYGUI_CONTROLLER_FADE_ALPHA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	/** This controller used for smooth changing alpha of widget in time */
	class MYGUI_EXPORT ControllerFadeAlpha :
		public ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerFadeAlpha )

	public:
		ControllerFadeAlpha();
		virtual ~ControllerFadeAlpha();

		/**
			@param _value Alpha that will be as result of changing
		*/
		void setAlpha(float _value);

		/**
			@param _value Coefficient of alpha changing speed (1. mean that alpha will change from 0 to 1 at 1 second)
		*/
		void setCoef(float _value);

		/**
			@param _value If true then widget will be inactive after start of alpha changing
		*/
		void setEnabled(bool _value);

		virtual void setProperty(const std::string& _key, const std::string& _value);

	private:
		bool addTime(Widget* _widget, float _time);
		void prepareItem(Widget* _widget);

	private:
		float mAlpha;
		float mCoef;
		bool mEnabled;
	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_FADE_ALPHA_H__
