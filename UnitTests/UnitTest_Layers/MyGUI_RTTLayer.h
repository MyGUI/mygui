/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#ifndef MYGUI_RTT_LAYER_H_
#define MYGUI_RTT_LAYER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"
#include "SceneObject.h"

namespace MyGUI
{

	class RTTLayer :
		public OverlappedLayer
#ifdef MYGUI_OGRE_PLATFORM
		, public demo::SceneObject
#endif
	{
		MYGUI_RTTI_DERIVED( RTTLayer )

	public:
		RTTLayer();
		virtual ~RTTLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;

		virtual IntPoint getPosition(int _left, int _top) const;

		void setTextureSize(const IntSize& _size);

	private:
		MyGUI::ITexture* mTexture;
		mutable IntPoint mOldPoint;
		IntSize mTextureSize;
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_H_
