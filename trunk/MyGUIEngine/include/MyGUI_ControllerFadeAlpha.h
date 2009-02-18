/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
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
#ifndef __MYGUI_CONTROLLER_FADE_ALPHA_H__
#define __MYGUI_CONTROLLER_FADE_ALPHA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	/** This controller used for smooth changing alpha of widget in time */
	class MYGUI_EXPORT ControllerFadeAlpha : public ControllerItem
	{
	public:
		/**
			@param _alpha that will be as result of changing
			@param _coef of alpha changing speed (1. mean that alpha will change from 0 to 1 at 1 second)
			@param _enabled if true then widget will be inactive after start of alpha changing
		*/
		ControllerFadeAlpha(float _alpha, float _coef, bool _enabled);

	private:
		ControllerFadeAlpha();

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);

		bool getEnabled() {return mEnabled;}
		float getAlpha() {return mAlpha;}
		float getCoef() {return mCoef;}

	private:
		float mAlpha;
		float mCoef;
		bool mEnabled;

	};

}

#endif // __MYGUI_CONTROLLER_FADE_ALPHA_H__
