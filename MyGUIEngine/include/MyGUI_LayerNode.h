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
	using VectorRenderItem = std::vector<RenderItem*>;
	using VectorLayerItem = std::vector<ILayerItem*>;

	class MYGUI_EXPORT LayerNode : public ILayerNode
	{
		MYGUI_RTTI_DERIVED(LayerNode)

	public:
		explicit LayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);
		~LayerNode() override;

		// леер, которому мы принадлежим
		ILayer* getLayer() const override;

		// возвращает отца или nullptr
		ILayerNode* getParent() const override;

		ILayerNode* createChildItemNode() override;
		void destroyChildItemNode(ILayerNode* _node) override;

		// up child item (make it draw and pick above others)
		void upChildItemNode(ILayerNode* _item) override;

		// child items list
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

		float getNodeDepth() const override;

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

		size_t mLastNotEmptyItem{0};

		// root widgets list
		// overlapping layers have only one item here
		VectorLayerItem mLayerItems;

		VectorILayerNode mChildItems;

		ILayerNode* mParent;
		ILayer* mLayer;
		bool mOutOfDate{false};
		bool mOutOfDateCompression{false};
		float mDepth{0.0f};
	};

} // namespace MyGUI

#endif // MYGUI_LAYER_NODE_H_
