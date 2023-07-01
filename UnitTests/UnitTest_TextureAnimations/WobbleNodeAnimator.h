/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#ifndef WOBBLE_NODE_ANIMATOR_H_
#define WOBBLE_NODE_ANIMATOR_H_

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_RTTLayerNode.h"
#include "Tesselator.h"

namespace demo
{

	class WobbleNodeAnimator : public MyGUI::LayerNodeAnimation, public Tesselator
	{
		MYGUI_RTTI_DERIVED(WobbleNodeAnimator)

	public:
		void attach(MyGUI::ILayerNode* _node) override;

		void create() override;
		void destroy() override;

		size_t animate(
			bool _update,
			size_t _quad_count,
			MyGUI::VectorQuadData& _data,
			float _time,
			MyGUI::IVertexBuffer* _buffer,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord,
			bool& _isAnimate) override;

	private:
		void addInertia(const MyGUI::FloatPoint& _value);
		void addTime(float _time);
		void buildQuadVertex(MyGUI::VectorQuadData& _data);

		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

	private:
		MyGUI::IntCoord mOldCoord;

		MyGUI::FloatPoint mInertia;
		MyGUI::FloatPoint mInertiaPoint;
		MyGUI::FloatPoint mDragOffset;

		bool mInertiaMode{false};
		float mDragStrength{0.001f};
		float mResizeStrength{0.0009f};
		bool mDestroy{true};
	};

}

#endif // WOBBLE_NODE_ANIMATOR_H_
