/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#ifndef FADE_NODE_ANIMATOR_H_
#define FADE_NODE_ANIMATOR_H_

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_RTTLayerNode.h"
#include "Tesselator.h"

namespace demo
{

	class FadeNodeAnimator : public MyGUI::LayerNodeAnimation, public Tesselator
	{
		MYGUI_RTTI_DERIVED(FadeNodeAnimator)

	public:
		FadeNodeAnimator();

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
		float mFadeDuration{0};
		int mFadeType{0};

		bool mDestroy{true};
		float mAlpha{0};
		std::vector<int> mSpeed;
	};

}

#endif // FADE_NODE_ANIMATOR_H_
