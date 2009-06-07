/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#ifndef __CUSTOM_LAYER_NODE_ANIMATION_H__
#define __CUSTOM_LAYER_NODE_ANIMATION_H__

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_GenericFactoryItem.h"

#include "Ogre.h"

namespace demo
{

	class CustomLayerNodeAnimation :
		public MyGUI::LayerNodeAnimation,
		public MyGUI::GenericFactoryItem<MyGUI::LayerNodeAnimation, CustomLayerNodeAnimation>
	{
	public:
		CustomLayerNodeAnimation(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);
		CustomLayerNodeAnimation();

		virtual size_t animate(
			bool _update,
			float _time,
			MyGUI::IVertexBuffer* _buffer,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord
			);

	private:
		void addInertia(const MyGUI::FloatPoint& _value);
		void addTime(float _time);
		void buildQuadVertex(const MyGUI::FloatCoord& _coord, float _z, MyGUI::VertexQuad * _quad, int _count_w, int _count_h, float _u, float _v, bool _flipY);

	private:
		MyGUI::IntCoord mOldCoord;

		MyGUI::FloatPoint mInertia;
		MyGUI::FloatPoint mInertiaPoint;
		MyGUI::FloatPoint mDragOffset;

		bool mInertiaMode;
		float mDragStrength;
		float mResizeStrength;
		bool mNeedUpdate;
	};

}

#endif // __CUSTOM_LAYER_NODE_ANIMATION_H__
