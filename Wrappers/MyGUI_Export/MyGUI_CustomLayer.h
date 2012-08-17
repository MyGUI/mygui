/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _65e1b826_c9fe_44a2_a771_1d9498291ad9_
#define _65e1b826_c9fe_44a2_a771_1d9498291ad9_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_SharedLayerNode.h"

namespace MyGUI
{

	class CustomLayer :
		public ILayer
	{
		MYGUI_RTTI_DERIVED( CustomLayer )

	public:
		CustomLayer();
		virtual ~CustomLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		virtual ILayerNode* createChildItemNode();

		virtual void destroyChildItemNode(ILayerNode* _node);

		virtual void upChildItemNode(ILayerNode* _node);

		virtual EnumeratorILayerNode getEnumerator() const;

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		virtual IntPoint getPosition(int _left, int _top) const;

		virtual const IntSize& getSize() const;

		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual void resizeView(const IntSize& _viewSize);

		bool isOutOfDate() const;

	protected:
		bool mIsPick;
		SharedLayerNode* mChildItem;
		bool mOutOfDate;
		IntSize mViewSize;
	};

}

#endif
