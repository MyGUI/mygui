/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LAYER_NODE_H_
#define MYGUI_LAYER_NODE_H_

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
		~LayerNode() override;

		// леер, которому мы принадлежим
		ILayer* getLayer() const override;

		// возвращает отца или nullptr
		ILayerNode* getParent() const override;

		// создаем дочерний нод
		ILayerNode* createChildItemNode() override;
		// удаляем дочерний нод
		void destroyChildItemNode(ILayerNode* _node) override;

		// поднимаем дочерний нод
		void upChildItemNode(ILayerNode* _node) override;

		// список детей
		EnumeratorILayerNode getEnumerator() const override;

		size_t getLayerNodeCount() const override;

		ILayerNode* getLayerNodeAt(size_t _index) const override;

		// добавляем айтем к ноду
		void attachLayerItem(ILayerItem* _item) override;
		// удаляем айтем из нода
		void detachLayerItem(ILayerItem* _item) override;

		// добавляет саб айтем и возвращает рендер айтем
		RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender) override;
		// необходимо обновление нода
		void outOfDate(RenderItem* _item) override;

		// возвращает виджет по позиции
		ILayerItem* getLayerItemByPoint(int _left, int _top) const override;

		// рисует леер
		void renderToTarget(IRenderTarget* _target, bool _update) override;

		void resizeView(const IntSize& _viewSize) override;

		float getNodeDepth() override;

		bool isOutOfDate() const;

	protected:
		// push all empty buffers to the end of buffers list
		void updateCompression();
		RenderItem* addToRenderItemFirstQueue(ITexture* _texture, bool _manualRender);
		RenderItem* addToRenderItemSecondQueue(ITexture* _texture, bool _manualRender);
	protected:
		// two render queues, for subskins and text
		// first queue keep render order based on order of creation
		// second queue ignore creation order and always merge render items with same texture
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

		size_t mLastNotEmptyItem;

		// root widgets list
		// overlapping layers have only one item here
		VectorLayerItem mLayerItems;

		VectorILayerNode mChildItems;

		ILayerNode* mParent;
		ILayer* mLayer;
		bool mOutOfDate;
		bool mOutOfDateCompression;
		float mDepth;
	};

} // namespace MyGUI

#endif // MYGUI_LAYER_NODE_H_
