/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#ifndef __MYGUI_I_LAYER_NODE_H__
#define __MYGUI_I_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_IObject.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class ILayer;
	class ILayerItem;
	class ILayerNode;

	class RenderItem;

	typedef std::vector<ILayerNode*> VectorILayerNode;
	typedef Enumerator<VectorILayerNode> EnumeratorILayerNode;

	class MYGUI_EXPORT ILayerNode :
		public IObject
	{
		MYGUI_RTTI_DERIVED( ILayerNode )

	public:
		virtual ~ILayerNode() { }

		// леер, которому мы принадлежим
		virtual ILayer* getLayer() const = 0;

		// возвращает отца или nullptr
		virtual ILayerNode* getParent() const = 0;

		// создаем дочерний нод
		virtual ILayerNode* createChildItemNode() = 0;
		// удаляем дочерний нод
		virtual void destroyChildItemNode(ILayerNode* _node) = 0;

		// поднимаем дочерний нод
		virtual void upChildItemNode(ILayerNode* _node) = 0;

		// список детей
		virtual EnumeratorILayerNode getEnumerator() const = 0;

		virtual size_t getLayerNodeCount() const = 0;

		virtual ILayerNode* getLayerNodeAt(size_t _index) const = 0;

		// добавляем айтем к ноду
		virtual void attachLayerItem(ILayerItem* _item) = 0;
		// удаляем айтем из нода
		virtual void detachLayerItem(ILayerItem* _root) = 0;

		// добавляет саб айтем и возвращает рендер айтем
		virtual RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _separate) = 0;
		// необходимо обновление нода
		virtual void outOfDate(RenderItem* _item) = 0;

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

		virtual void resizeView(const IntSize& _viewSize) = 0;

		virtual float getNodeDepth() = 0;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_NODE_H__
