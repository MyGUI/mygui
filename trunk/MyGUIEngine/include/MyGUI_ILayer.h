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
#ifndef __MYGUI_I_LAYER_H__
#define __MYGUI_I_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class ILayerItem;
	class ILayerNode;

	class MYGUI_EXPORT ILayer
	{
	public:
		ILayer(const std::string& _name) : mName(_name) { }
		virtual ~ILayer() = 0 { }

		// имя леера
		const std::string& getName() { return mName; }

		// создает узел
		virtual ILayerNode* createItemNode(ILayerNode* _parent) = 0;
		// удаляет узел
		virtual void destroyItemNode(ILayerNode* _item) = 0;
		// существует ли узел
		virtual bool existItemNode(ILayerNode* _item) = 0;
		// поднимает узел над своими родствениками
		virtual void upItemNode(ILayerNode* _item) = 0;

		// рисует леер
		virtual void doRender(bool _update) = 0;

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) = 0;

	private:
		std::string mName;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_H__
