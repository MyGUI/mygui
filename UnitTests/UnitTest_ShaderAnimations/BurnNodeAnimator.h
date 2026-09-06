/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#ifndef BURN_NODE_ANIMATOR_H_
#define BURN_NODE_ANIMATOR_H_

#include "MyGUI_LayerNodeAnimation.h"

namespace demo
{

	class BurnNodeAnimator : public MyGUI::LayerNodeAnimation
	{
		MYGUI_RTTI_DERIVED(BurnNodeAnimator)

	public:
		BurnNodeAnimator();

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
		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

	private:
		float mBurnDuration{6.0f};
		float mNoiseScale{9.0f};
		float mTime{0.0f};
	};

}

#endif // BURN_NODE_ANIMATOR_H_
