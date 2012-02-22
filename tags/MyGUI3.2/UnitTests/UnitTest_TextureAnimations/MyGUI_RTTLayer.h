/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#ifndef __MYGUI_RTT_LAYER_H__
#define __MYGUI_RTT_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */RTTLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( RTTLayer )

	public:
		RTTLayer();
		virtual ~RTTLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// ������� �������� ���
		virtual ILayerNode* createChildItemNode();
		// ������� �������� ���
		virtual void destroyChildItemNode(ILayerNode* _node);

		// ���������� ������ �� �������
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		// ������ ����
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

	private:
		Version mVersion;
		xml::ElementPtr mData;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_H__
