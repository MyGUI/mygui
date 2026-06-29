/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_LAYER_NODE_H_
#define MYGUI_I_LAYER_NODE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_IObject.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_Types.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class ILayer;
	class ILayerItem;
	class ILayerNode;

	class RenderItem;

	using VectorILayerNode = std::vector<ILayerNode*>;

	class MYGUI_EXPORT ILayerNode : public IObject, public MemberObsolete<ILayerNode>
	{
		MYGUI_RTTI_DERIVED(ILayerNode)

	public:
		// Layer we belong to
		virtual ILayer* getLayer() const = 0;

		// Return parent or nullptr
		virtual ILayerNode* getParent() const = 0;

		virtual ILayerNode* createChildItemNode() = 0;
		virtual void destroyChildItemNode(ILayerNode* _node) = 0;

		// up child item (make it draw and pick above others)
		virtual void upChildItemNode(ILayerNode* _item) = 0;

		// child items list
		virtual const VectorILayerNode& getChildItems() const = 0;

		virtual size_t getLayerNodeCount() const = 0;

		virtual ILayerNode* getLayerNodeAt(size_t _index) const = 0;

		virtual void attachLayerItem(ILayerItem* _item) = 0;
		virtual void detachLayerItem(ILayerItem* _root) = 0;

		virtual RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _separate) = 0;
		virtual void outOfDate(RenderItem* _item) = 0;

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;

		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

		virtual void resizeView(const IntSize& _viewSize) = 0;

		virtual float getNodeDepth() const = 0;
	};

} // namespace MyGUI

#endif // MYGUI_I_LAYER_NODE_H_
