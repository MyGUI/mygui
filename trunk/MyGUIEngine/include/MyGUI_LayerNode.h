/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_LAYER_NODE_H__
#define __MYGUI_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_RenderItem.h"

namespace MyGUI
{

	class LayerItem;
	typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<ILayerItem*> VectorLayerItem;

	class MYGUI_EXPORT LayerNode :
		public ILayerNode
	{
		MYGUI_RTTI_DERIVED( LayerNode )

	public:
		explicit LayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);
		virtual ~LayerNode();

		// леер, которому мы принадлежим
		virtual ILayer* getLayer() const;

		// возвращает отца или nullptr
		virtual ILayerNode* getParent() const;

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

		// добавляем айтем к ноду
		virtual void attachLayerItem(ILayerItem* _item);
		// удаляем айтем из нода
		virtual void detachLayerItem(ILayerItem* _item);

		// добавляет саб айтем и возвращает рендер айтем
		virtual RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender);
		// необходимо обновление нода
		virtual void outOfDate(RenderItem* _item);

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual void resizeView(const IntSize& _viewSize);

		virtual float getNodeDepth();

		bool isOutOfDate() const;

	protected:
		void updateCompression();

	protected:
		// список двух очередей отрисовки, для сабскинов и текста
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		VectorLayerItem mLayerItems;

		// список такиж как мы, для построения дерева
		VectorILayerNode mChildItems;

		ILayerNode* mParent;
		ILayer* mLayer;
		bool mOutOfDate;
		float mDepth;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_NODE_H__
