/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#ifndef __FADE_NODE_ANIMATOR_H__
#define __FADE_NODE_ANIMATOR_H__

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_RTTLayerNode.h"
#include "Tesselator.h"

namespace demo
{

	class FadeNodeAnimator :
		public MyGUI::LayerNodeAnimation,
		public Tesselator
	{
		MYGUI_RTTI_DERIVED( FadeNodeAnimator )

	public:
		FadeNodeAnimator();

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
		void addTime(float _time);

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

	private:
		float mFadeDuration;
		int mFadeType;

		bool mDestroy;
		float mAlpha;
		std::vector<int> mSpeed;

	};

}

#endif // __FADE_NODE_ANIMATOR_H__
