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
		virtual ~SharedLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		void setPick(bool _pick);

		virtual ILayerNode* createChildItemNode();
		virtual void destroyChildItemNode(ILayerNode* _node);

		// up child item (make it draw and pick above other)
		virtual void upChildItemNode(ILayerNode* _node);

		// child items list
		virtual EnumeratorILayerNode getEnumerator() const;

		virtual size_t getLayerNodeCount() const;

		virtual ILayerNode* getLayerNodeAt(size_t _index) const;

		// return widget at position
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		// return position in layer coordinates
		virtual IntPoint getPosition(int _left, int _top) const;

		// return layer size
		virtual const IntSize& getSize() const;

		// render layer
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

#endif // MYGUI_SHARED_LAYER_H_
