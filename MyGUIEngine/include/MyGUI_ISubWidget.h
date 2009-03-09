/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#ifndef __MYGUI_I_SUB_WIDGET_H__
#define __MYGUI_I_SUB_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Rtti.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_DrawItem.h"

namespace MyGUI
{

	// базовый тип информации о стейте
	class StateInfo{};
	typedef std::vector<StateInfo*> VectorStateInfo;
	typedef std::map<std::string, StateInfo*> MapStateInfo;
	typedef std::map<std::string, VectorStateInfo> MapWidgetStateInfo;

	class ISubWidget;
	typedef std::vector<ISubWidget*> VectorSubWidget;

	class LayerItemKeeper;
	class RenderItem;


	class MYGUI_EXPORT ISubWidget : public ICroppedRectangle, public DrawItem
	{
		MYGUI_RTTI_BASE_HEADER ( ISubWidget );

	public:
		ISubWidget(const IntCoord & _coord, Align _align, ICroppedRectangle * _parent) :
			ICroppedRectangle(_coord, _align, _parent) { }
			virtual ~ISubWidget() { }

		virtual void setAlpha(float _alpha) = 0;
		virtual float getAlpha() { return 1.0; }

		virtual void _setStateData(StateInfo * _data) { }

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item) { }
		virtual void _destroyDrawItem() { }

		virtual bool firstQueue() = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_H__
