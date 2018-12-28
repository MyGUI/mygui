/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SHARED_LAYER_H_
#define MYGUI_SHARED_LAYER_H_

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
		~SharedLayer() override;

		void deserialization(xml::ElementPtr _node, Version _version) override;

		void setPick(bool _pick);

		ILayerNode* createChildItemNode() override;
		void destroyChildItemNode(ILayerNode* _node) override;

		// up child item (make it draw and pick above other)
		void upChildItemNode(ILayerNode* _node) override;

		// child items list
		EnumeratorILayerNode getEnumerator() const override;

		size_t getLayerNodeCount() const override;

		ILayerNode* getLayerNodeAt(size_t _index) const override;

		// return widget at position
		ILayerItem* getLayerItemByPoint(int _left, int _top) const override;

		// return position in layer coordinates
		IntPoint getPosition(int _left, int _top) const override;

		// return layer size
		const IntSize& getSize() const override;

		// render layer
		void renderToTarget(IRenderTarget* _target, bool _update) override;

		void resizeView(const IntSize& _viewSize) override;

		bool isOutOfDate() const;

	protected:
		bool mIsPick;
		SharedLayerNode* mChildItem;
		bool mOutOfDate;
		IntSize mViewSize;
	};

} // namespace MyGUI

#endif // MYGUI_SHARED_LAYER_H_
