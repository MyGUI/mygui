/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#include "BurnNodeAnimator.h"

#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Gui.h"

#include <algorithm>
#include <cmath>

namespace demo
{

	BurnNodeAnimator::BurnNodeAnimator() = default;

	void BurnNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			std::string_view key = node->findAttribute("key");
			std::string_view value = node->findAttribute("value");

			if (key == "BurnDuration")
				mBurnDuration = MyGUI::utility::parseFloat(value);
			else if (key == "NoiseScale")
				mNoiseScale = MyGUI::utility::parseFloat(value);
		}

		if (mBurnDuration <= 0.0f)
			mBurnDuration = 6.0f;
	}

	size_t BurnNodeAnimator::animate(
		bool _update,
		size_t _quad_count,
		MyGUI::VectorQuadData& _data,
		float _time,
		MyGUI::IVertexBuffer* _buffer,
		MyGUI::ITexture* _texture,
		const MyGUI::RenderTargetInfo& _info,
		const MyGUI::IntCoord& _coord,
		bool& _isAnimate)
	{
		if (_texture == nullptr || _texture->getWidth() == 0 || _texture->getHeight() == 0)
			return _quad_count;

		mTime += _time;

		// ping-pong: burn out, then restore
		float phase = std::fmod(mTime / mBurnDuration, 2.0f);
		float progress = phase <= 1.0f ? phase : 2.0f - phase;
		progress = std::clamp(progress, 0.0f, 1.0f);

		// texture can be recreated at any time, so set shader every frame
		_texture->setShader("BurnShader");

		// texture is power of two sized and bigger than content, so TexCoord does not reach 1.0
		float dpiScale = MyGUI::Gui::getInstance().getDpiScale();
		float texCoordMaxU = (float)_coord.width * dpiScale / (float)_texture->getWidth();
		float texCoordMaxV = (float)_coord.height * dpiScale / (float)_texture->getHeight();

		_texture->setShaderParams(
			{{"Progress", progress}, {"TexCoordMax", texCoordMaxU, texCoordMaxV}, {"NoiseScale", mNoiseScale}});

		_isAnimate = true;

		return _quad_count;
	}

	void BurnNodeAnimator::create()
	{
		mTime = 0.0f;
	}

	void BurnNodeAnimator::destroy()
	{
	}

	void BurnNodeAnimator::attach(MyGUI::ILayerNode* _node)
	{
		_node->castType<MyGUI::RTTLayerNode>()->addLayerNodeAnimation(this);
	}

}
