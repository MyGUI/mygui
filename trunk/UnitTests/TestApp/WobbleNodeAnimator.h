/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#ifndef __WOBBLE_NODE_ANIMATOR_H__
#define __WOBBLE_NODE_ANIMATOR_H__

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_RTTLayerNode.h"

namespace demo
{

	class WoobleNodeAnimator :
		public MyGUI::LayerNodeAnimation
	{
	public:
		WoobleNodeAnimator();

		virtual void attach(MyGUI::ILayerNode* _node);

		virtual void create();
		virtual void destroy();

		virtual size_t animate(
			bool _update,
			size_t _quad_count,
			MyGUI::VectorQuadData& _data,
			float _time,
			MyGUI::IVertexBuffer* _buffer,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord,
			bool& _isAnimate
			);

	private:
		void addInertia(const MyGUI::FloatPoint& _value);
		void addTime(float _time);
		void buildQuadVertex(const MyGUI::FloatCoord& _coord, float _z, MyGUI::VectorQuadData& _data, int _count_w, int _count_h, float _u, float _v, bool _flipY);

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

	private:
		MyGUI::IntCoord mOldCoord;

		MyGUI::FloatPoint mInertia;
		MyGUI::FloatPoint mInertiaPoint;
		MyGUI::FloatPoint mDragOffset;

		bool mInertiaMode;
		float mDragStrength;
		float mResizeStrength;
		bool mDestroy;

		MyGUI::RTTLayerNode* mNode;
	};

}

#endif // __WOBBLE_NODE_ANIMATOR_H__
