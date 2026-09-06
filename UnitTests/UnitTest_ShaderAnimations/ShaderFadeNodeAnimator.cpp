/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#include "ShaderFadeNodeAnimator.h"

#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Gui.h"

namespace demo
{

	void ShaderFadeNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			std::string_view key = node->findAttribute("key");
			std::string_view value = node->findAttribute("value");

			if (key == "FadeDuration")
				mFadeDuration = MyGUI::utility::parseFloat(value);
			else if (key == "FadeType")
				mFadeType = MyGUI::utility::parseInt(value); //FIXME
		}

		if (mFadeDuration <= 0.0f)
			mFadeDuration = 1.5f;
	}

	size_t ShaderFadeNodeAnimator::animate(
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

		addTime(_time);

		if (mDestroy && mAlpha <= 0.0f)
			return _quad_count;

		_isAnimate = true;

		// texture can be recreated at any time, so set shader every frame
		_texture->setShader("FadeShader");

		float dpiScale = MyGUI::Gui::getInstance().getDpiScale();
		float texCoordMaxU = (float)_coord.width * dpiScale / (float)_texture->getWidth();
		float texCoordMaxV = (float)_coord.height * dpiScale / (float)_texture->getHeight();

		_texture->setShaderParams(
			{{"Alpha", mAlpha}, {"FadeType", (float)mFadeType}, {"TexCoordMax", texCoordMaxU, texCoordMaxV}});

		return _quad_count;
	}

	void ShaderFadeNodeAnimator::addTime(float _time)
	{
		if (mDestroy)
		{
			if (mAlpha > 0)
			{
				mAlpha -= _time / mFadeDuration;
				if (mAlpha < 0)
					mAlpha = 0;
			}
		}
		else
		{
			if (mAlpha < 1)
			{
				mAlpha += _time / mFadeDuration;
				if (mAlpha > 1)
					mAlpha = 1;
			}
		}
	}

	void ShaderFadeNodeAnimator::create()
	{
		mDestroy = false;
		mAlpha = 0;
	}

	void ShaderFadeNodeAnimator::destroy()
	{
		mDestroy = true;
	}

	void ShaderFadeNodeAnimator::attach(MyGUI::ILayerNode* _node)
	{
		_node->castType<MyGUI::RTTLayerNode>()->addLayerNodeAnimation(this);
	}

}
