/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#include "FadeNodeAnimator.h"

namespace demo
{

	FadeNodeAnimator::FadeNodeAnimator() :
		mFadeDuration(0),
		mFadeType(0),
		mDestroy(true),
		mAlpha(0)
	{
		mSpeed.resize(getCount());
	}

	void FadeNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			const std::string& key = node->findAttribute("key");
			const std::string& value = node->findAttribute("value");

			if (key == "FadeDuration") mFadeDuration = MyGUI::utility::parseFloat(value);
			else if (key == "FadeType") mFadeType = MyGUI::utility::parseInt(value);//FIXME
		}
	}

	size_t FadeNodeAnimator::animate(
		bool _update,
		size_t _quad_count,
		MyGUI::VectorQuadData& _data,
		float _time,
		MyGUI::IVertexBuffer* _buffer,
		MyGUI::ITexture* _texture,
		const MyGUI::RenderTargetInfo& _info,
		const MyGUI::IntCoord& _coord,
		bool& _isAnimate
	)
	{

		addTime(_time);

		if (mDestroy && mAlpha == 0)
		{
			return _quad_count;
		}

		_isAnimate = true;

		_quad_count = tesselation(
			_quad_count,
			_data,
			_texture,
			_info,
			_coord);

		size_t index = 0;
		switch (mFadeType)
		{
		case 0: // random squares
			for (; index < _quad_count; ++index)
			{
				float alpha = pow(mAlpha, mSpeed[index]);

				unsigned int colour = 0xFFFFFF | ((unsigned int)(alpha * 255.0f) << 24);

				_data[index].vertex[MyGUI::QuadData::CornerLT].colour = colour;
				_data[index].vertex[MyGUI::QuadData::CornerRT].colour = colour;
				_data[index].vertex[MyGUI::QuadData::CornerLB].colour = colour;
				_data[index].vertex[MyGUI::QuadData::CornerRB].colour = colour;
			}
			break;
		case 1: // random not-squares
		case 2: // from center
		case 3: // TV
			for (int y = 0; y < getCountY() + 1; ++y)
			{
				for (int x = 0; x < getCountX() + 1; ++x)
				{
					index = x + y * getCountX();
					float speed = 1.0f;
					if (mFadeType == 1) // random not-squares
					{
						speed = (float)mSpeed[index % (getCountX() * getCountY())];
					}
					if (mFadeType == 2) // from center
					{
						float x1 = float(x - getCountX() / 2) / getCountX() * 2;
						float y1 = float(y - getCountY() / 2) / getCountY() * 2;
						speed = sqrt(x1 * x1 + y1 * y1);
					}
					if (mFadeType == 3) // TV
					{
						float x1 = float(x - getCountX() / 2) / getCountX() * 2;
						float y1 = float(y - getCountY() / 2) / getCountY() * 2;
						speed = sqrt(x1 * x1 * x1 * x1 + y1 * y1);
					}
					float alpha = pow(mAlpha, speed);

					unsigned int colour = 0xFFFFFF | ((unsigned int)(alpha * 255.0f) << 24);

					if ( x < getCountX() && y < getCountY() ) _data[index].vertex[MyGUI::QuadData::CornerLT].colour = colour;
					if ( x > 0       && y < getCountY() ) _data[index - 1].vertex[MyGUI::QuadData::CornerRT].colour = colour;
					if ( x < getCountX() && y > 0       ) _data[index - getCountX()].vertex[MyGUI::QuadData::CornerLB].colour = colour;
					if ( x > 0       && y > 0       ) _data[index - getCountX() - 1].vertex[MyGUI::QuadData::CornerRB].colour = colour;
				}
			}
			break;
		}

		return _quad_count;
	}

	void FadeNodeAnimator::addTime(float _time)
	{
		if (mDestroy)
		{
			if (mAlpha > 0)
			{
				mAlpha -= _time / mFadeDuration;
				if (mAlpha < 0) mAlpha = 0;
			}
		}
		else
		{
			if (mAlpha < 1)
			{
				mAlpha += _time / mFadeDuration;
				if (mAlpha > 1) mAlpha = 1;
			}
		}

	}

	void FadeNodeAnimator::create()
	{
		mDestroy = false;
		mAlpha = 0;

		int count = getCount();

		for (int index = 0; index < count; ++index)
			mSpeed[index] = ::rand() % 4 + 1;
	}

	void FadeNodeAnimator::destroy()
	{
		mDestroy = true;
	}

	void FadeNodeAnimator::attach(MyGUI::ILayerNode* _node)
	{
		_node->castType<MyGUI::RTTLayerNode>()->addLayerNodeAnimation(this);
	}

}
