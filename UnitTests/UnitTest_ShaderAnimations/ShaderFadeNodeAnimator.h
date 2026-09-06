/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#ifndef SHADER_FADE_NODE_ANIMATOR_H_
#define SHADER_FADE_NODE_ANIMATOR_H_

#include "MyGUI_LayerNodeAnimation.h"

namespace demo
{

	// same as FadeNodeAnimator from UnitTest_TextureAnimations, but per-pixel fade
	// is computed in fragment shader instead of tessellation and vertex colours on CPU
	class ShaderFadeNodeAnimator : public MyGUI::LayerNodeAnimation
	{
		MYGUI_RTTI_DERIVED(ShaderFadeNodeAnimator)

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
		void addTime(float _time);

		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

	private:
		float mFadeDuration{1.5f};
		int mFadeType{0};

		bool mDestroy{true};
		float mAlpha{0};
	};

}

#endif // SHADER_FADE_NODE_ANIMATOR_H_
