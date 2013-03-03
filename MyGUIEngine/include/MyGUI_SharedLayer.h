/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SHARED_LAYER_H__
#define __MYGUI_SHARED_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_SharedLayerNode.h"

namespace MyGUI
{

	class MYGUI_EXPORT SharedLayer :
		public ILayer
	{
		MYGUI_RTTI_DERIVED( SharedLayer )

	public:
		SharedLayer();
		virtual ~SharedLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// создаем дочерний нод
		virtual ILayerNode* createChildItemNode();
		// удаляем дочерний нод
		virtual void destroyChildItemNode(ILayerNode* _node);

		// поднимаем дочерний нод
		virtual void upChildItemNode(ILayerNode* _node);

		// список детей
		virtual EnumeratorILayerNode getEnumerator() const;

		virtual size_t getLayerNodeCount() const;

		virtual ILayerNode* getLayerNodeAt(size_t _index) const;

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		virtual IntPoint getPosition(int _left, int _top) const;

		virtual const IntSize& getSize() const;

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual void resizeView(const IntSize& _viewSize);

		bool isOutOfDate() const;

	protected:
		bool mIsPick;
		SharedLayerNode* mChildItem;
		bool mOutOfDate;
		IntSize mViewSize;
	};

} // namespace MyGUI

#endif // __MYGUI_SHARED_LAYER_H__
