/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_LAYER_H_
#define MYGUI_I_LAYER_H_

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

		const std::string& getName() const
		{
			return mName;
		}

		void setName(const std::string& _name)
		{
			mName = _name;
		}

		virtual ILayerNode* createChildItemNode() = 0;
		virtual void destroyChildItemNode(ILayerNode* _node) = 0;

		// up child item (make it draw and pick above other)
		virtual void upChildItemNode(ILayerNode* _node) = 0;

		// child items list
		virtual EnumeratorILayerNode getEnumerator() const = 0;

		virtual size_t getLayerNodeCount() const = 0;

		virtual ILayerNode* getLayerNodeAt(size_t _index) const = 0;

		// return widget at position
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;

		// return position in layer coordinates
		virtual IntPoint getPosition(int _left, int _top) const = 0;

		// return layer size
		virtual const IntSize& getSize() const = 0;

		// render layer
		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

		virtual void resizeView(const IntSize& _viewSize) = 0;

	protected:
		std::string mName;
	};

} // namespace MyGUI

#endif // MYGUI_I_LAYER_H_
