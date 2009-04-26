/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#ifndef __MYGUI_I_LAYER_NODE_H__
#define __MYGUI_I_LAYER_NODE_H__

#include <string>

namespace MyGUI
{

	class ILayerItem;
	class IRenderItem;
	class ILayer;
	class IDrawItem;

	class /*MYGUI_EXPORT*/ ILayerNode
	{
	public:
		virtual ~ILayerNode() = 0 { }

		// леер, которому мы принадлежим
		virtual ILayer* getLayer() = 0;

		// создаем дочерний нод
		virtual ILayerNode* createItemNode() = 0;

		// удаляем себя и все дочерние ноды
		virtual void destroyItemNode() = 0;

		// добавляем айтем к ноду
		virtual void attachLayerItem(ILayerItem* _item) = 0;
		// удаляем айтем из нода
		virtual void detachLayerItem(ILayerItem* _root) = 0;

		// поднимаем себя над другими нодами
		virtual void upItemNode() = 0;

		// добавляет саб айтем и возвращает рендер айтем
		virtual IRenderItem* addToRenderItem(const std::string& _texture, IDrawItem* _item) = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_NODE_H__
