/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#ifndef SHADER_WOBBLE_NODE_ANIMATOR_H_
#define SHADER_WOBBLE_NODE_ANIMATOR_H_

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_RTTLayerNode.h"
#include "Tesselator.h"

namespace demo
{

	// same as WobbleNodeAnimator from UnitTest_TextureAnimations, but grid vertices
	// are displaced in vertex shader instead of buildQuadVertex on CPU.
	// Wobble physics (inertia and drag offset) is still computed on CPU.
	class ShaderWobbleNodeAnimator : public MyGUI::LayerNodeAnimation, public Tesselator
	{
		MYGUI_RTTI_DERIVED(ShaderWobbleNodeAnimator)

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

		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

	private:
		float mDragStrength{0};
		float mResizeStrength{0};

		bool mDestroy{true};

		bool mInertiaMode{false};
		MyGUI::FloatPoint mInertiaPoint;
		MyGUI::FloatPoint mInertia;
		MyGUI::FloatPoint mDragOffset;
		MyGUI::IntCoord mOldCoord;
	};

}

#endif // SHADER_WOBBLE_NODE_ANIMATOR_H_
