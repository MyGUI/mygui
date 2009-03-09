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
#ifndef __MYGUI_CONTROLLER_MANAGER_H__
#define __MYGUI_CONTROLLER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_ControllerItem.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ActionController.h"

namespace MyGUI
{

	typedef std::pair<WidgetPtr, ControllerItem *> PairControllerItem;
	typedef std::list<PairControllerItem> ListControllerItem;

	class MYGUI_EXPORT ControllerManager : public IUnlinkWidget
	{
		MYGUI_INSTANCE_HEADER(ControllerManager);

	public:
		void initialise();
		void shutdown();

		/** Add controlled widget
			@param _widget to be controlled
			@param _item controller with some actions (for example ControllerFadeAlpha or your own)
			@note _item will be deleted automatically at end of controller lifetime
				(if not removed by removeItem(WidgetPtr _widget) before)
		*/
		void addItem(WidgetPtr _widget, ControllerItem * _item);
		/** Stop the control over a widget
			@param _widget to be removed
		*/
		void removeItem(WidgetPtr _widget);

		void _unlinkWidget(WidgetPtr _widget);

	private:
		void frameEntered(float _time);
		void clear();

	private:
		ListControllerItem mListItem;

	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_MANAGER_H__
