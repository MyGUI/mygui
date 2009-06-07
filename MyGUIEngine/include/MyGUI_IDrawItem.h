/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
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
#ifndef __MYGUI_I_DRAW_ITEM_H__
#define __MYGUI_I_DRAW_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_RTTI.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class ILayerNode;

	// базовый тип информации о стейте
	class StateInfo { };
	typedef std::vector<StateInfo*> VectorStateInfo;
	typedef std::map<std::string, StateInfo*> MapStateInfo;
	typedef std::map<std::string, VectorStateInfo> MapWidgetStateInfo;

	class IDrawItem;
	typedef std::vector<IDrawItem*> VectorIDrawItem;

	class MYGUI_EXPORT IDrawItem
	{
		MYGUI_RTTI_BASE ( IDrawItem );

	public:
		virtual ~IDrawItem() { }

		virtual void createDrawItem(const std::string& _texture, ILayerNode* _node) = 0;
		virtual void destroyDrawItem() = 0;

		virtual void setAlpha(float _alpha) { }

		virtual void setStateData(StateInfo* _data) { }

		virtual void doRender() = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_DRAW_ITEM_H__
