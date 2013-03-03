/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_LAYER_H__
#define __MYGUI_I_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_ISerializable.h"

namespace MyGUI
{

	class ILayerItem;

	class MYGUI_EXPORT ILayer :
		public ISerializable
	{
		MYGUI_RTTI_DERIVED( ILayer )

	public:
		ILayer() { }
		virtual ~ILayer() { }

		// имя леера
		const std::string& getName() const
		{
			return mName;
		}

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

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;

		// возвращает позицию в координатах леера
		virtual IntPoint getPosition(int _left, int _top) const = 0;

		// возвращает размер леера
		virtual const IntSize& getSize() const = 0;

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

		virtual void resizeView(const IntSize& _viewSize) = 0;

	protected:
		std::string mName;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_H__
