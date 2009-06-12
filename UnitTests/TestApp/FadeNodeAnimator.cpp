/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "FadeNodeAnimator.h"

namespace demo
{

	FadeNodeAnimator::FadeNodeAnimator() :
		mDestroy(true),
		mAlpha(0)
	{
		mSpeed.resize(getCount());
	}

	void FadeNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		/*MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			const std::string& key = node->findAttribute("key");
			const std::string& value = node->findAttribute("value");

			if (key == "DragStrength") mDragStrength = MyGUI::utility::parseFloat(value);
			else if (key == "ResizeStrength") mResizeStrength = MyGUI::utility::parseFloat(value);
		}*/
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

		for (size_t index=0; index<_quad_count; ++index)
		{
			float alpha = mAlpha / mSpeed[index];
			if (alpha > 1) alpha = 1;

			unsigned int colour = 0xFFFFFF | ((unsigned int)(alpha * 255.0f) << 24);

			_data[index].vertex[MyGUI::QuadData::CornerLT].colour = colour;
			_data[index].vertex[MyGUI::QuadData::CornerRT].colour = colour;
			_data[index].vertex[MyGUI::QuadData::CornerLB].colour = colour;
			_data[index].vertex[MyGUI::QuadData::CornerRB].colour = colour;
		}

		return _quad_count;
	}

	void FadeNodeAnimator::addTime(float _time)
	{
		if (mDestroy)
		{
			if (mAlpha > 0)
			{
				mAlpha -= _time * 3;
				if (mAlpha < 0) mAlpha = 0;
			}
		}
		else
		{
			if (mAlpha < 1)
			{
				mAlpha += _time * 3;
				if (mAlpha > 1) mAlpha = 1;
			}
		}

	}

	void FadeNodeAnimator::create()
	{
		mDestroy = false;
		mAlpha = 0;

		int count = getCount();

		for (int index=0; index<count; ++index)
			mSpeed[index] = ((float)(::rand() % 1000) / 999.0);
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