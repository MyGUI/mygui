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
		,
		public demo::SceneObject
#endif
	{
		MYGUI_RTTI_DERIVED(RTTLayer)

	public:
		~RTTLayer() override;

		void deserialization(xml::ElementPtr _node, Version _version) override;

		void renderToTarget(IRenderTarget* _target, bool _update) override;

		ILayerItem* getLayerItemByPoint(int _left, int _top) const override;

		IntPoint getPosition(int _left, int _top) const override;

		void setTextureSize(const IntSize& _size);

	private:
		MyGUI::ITexture* mTexture{nullptr};
		mutable IntPoint mOldPoint;
		IntSize mTextureSize;
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_H_
