/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LAYER_ITEM_H_
#define MYGUI_LAYER_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_ILayerItem.h"
#include "MyGUI_ISubWidget.h"

namespace MyGUI
{

	class MYGUI_EXPORT LayerItem : public ILayerItem
	{
	public:
		ILayer* getLayer() const;
		ILayerNode* getLayerNode() const;

		void attachItemToNode(ILayer* _layer, ILayerNode* _node) override;
		void detachFromLayer() override;
		void upLayerItem() override;

		void setRenderItemTexture(ITexture* _texture);

		void addChildItem(LayerItem* _item);
		void removeChildItem(LayerItem* _item);

		void addChildNode(LayerItem* _item);
		void removeChildNode(LayerItem* _item);

		void addRenderItem(ISubWidget* _item);
		void removeAllRenderItems();

		void saveLayerItem();
		void restoreLayerItem();

	protected:
		void attachToLayerItemNode(ILayerNode* _item, bool _deep);
		void detachFromLayerItemNode(bool _deep);

	private:
		// актуально для рутового виджета
		ILayer* mLayer{nullptr};
		// конкретный айтем находящийся в слое
		ILayerNode* mLayerNode{nullptr};
		ILayerNode* mSaveLayerNode{nullptr};

		using VectorLayerItem = std::vector<LayerItem*>;
		// список наших детей айтемов
		VectorLayerItem mLayerItems;
		// список наших узлов
		VectorLayerItem mLayerNodes;

		// вектор всех детей сабскинов
		VectorSubWidget mDrawItems;

		ITexture* mTexture{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_LAYER_ITEM_H_
