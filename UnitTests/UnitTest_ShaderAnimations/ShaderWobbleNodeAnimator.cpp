/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#include "ShaderWobbleNodeAnimator.h"

#include "MyGUI_Gui.h"

#include <cmath>

namespace demo
{

	static float getLength(const MyGUI::FloatPoint& _value)
	{
		return std::sqrt(_value.left * _value.left + _value.top * _value.top);
	}

	static void setLength(MyGUI::FloatPoint& _value, float _len)
	{
		float temp = getLength(_value);
		_value.left *= _len / temp;
		_value.top *= _len / temp;
	}

	static float squaredLength(const MyGUI::FloatPoint& _value)
	{
		return _value.left * _value.left + _value.top * _value.top;
	}

	void ShaderWobbleNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			std::string_view key = node->findAttribute("key");
			std::string_view value = node->findAttribute("value");

			if (key == "DragStrength")
				mDragStrength = MyGUI::utility::parseFloat(value);
			else if (key == "ResizeStrength")
				mResizeStrength = MyGUI::utility::parseFloat(value);
		}
	}

	size_t ShaderWobbleNodeAnimator::animate(
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

		// check widget offset
		if (mOldCoord.empty())
		{
			// do nothing
		}
		else if (mOldCoord.size() != _coord.size() && mOldCoord.point() != _coord.point())
		{
			mInertiaPoint.set(0.5, 0.5);
			mInertiaMode = false;

			addInertia(MyGUI::FloatPoint((float)_coord.left - mOldCoord.left, (float)_coord.top - mOldCoord.top));
		}
		else if (mOldCoord.size() != _coord.size())
		{
			mInertiaMode = true;

			addInertia(
				MyGUI::FloatPoint((float)_coord.width - mOldCoord.width, (float)_coord.height - mOldCoord.height));
		}
		else if (mOldCoord.point() != _coord.point())
		{
			const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
			mInertiaPoint = MyGUI::FloatPoint(
				(float)(point.left - _coord.left) / (float)_coord.width,
				(float)(point.top - _coord.top) / (float)_coord.height);
			mInertiaMode = false;

			addInertia(MyGUI::FloatPoint((float)_coord.left - mOldCoord.left, (float)_coord.top - mOldCoord.top));
		}

		mOldCoord = _coord;

		addTime(_time);

		bool anim_update = squaredLength(mDragOffset) >= 0.3f;

		if (!anim_update)
		{
			return _quad_count;
		}

		_isAnimate = true;

		_quad_count = tesselation(_quad_count, _data, _texture, _info, _coord);

		// texture can be recreated at any time, so set shader every frame
		_texture->setShader("WobbleShader");

		float dpiScale = MyGUI::Gui::getInstance().getDpiScale();
		float texCoordMaxU = (float)_coord.width * dpiScale / (float)_texture->getWidth();
		float texCoordMaxV = (float)_coord.height * dpiScale / (float)_texture->getHeight();

		_texture->setShaderParams(
			{{"DragOffset", mDragOffset.left, mDragOffset.top},
			 {"InertiaPoint", mInertiaPoint.left, mInertiaPoint.top},
			 {"InertiaMode", mInertiaMode ? 1.0f : 0.0f},
			 {"DragStrength", mDragStrength},
			 {"ResizeStrength", mResizeStrength},
			 {"TexCoordMax", texCoordMaxU, texCoordMaxV}});

		return _quad_count;
	}

	void ShaderWobbleNodeAnimator::addInertia(const MyGUI::FloatPoint& _value)
	{
		const float clampFactor = 50.0f;
		mInertia = mInertia + _value;
		if (getLength(mInertia) > clampFactor)
			setLength(mInertia, clampFactor);
	}

	void ShaderWobbleNodeAnimator::addTime(float _time)
	{
		const float speed = 4;
		_time = std::min(0.05f, _time);

		MyGUI::FloatPoint previousdrag = mDragOffset;

		mDragOffset.left = mDragOffset.left + mInertia.left * 5.0f * speed * _time;
		mDragOffset.top = mDragOffset.top + mInertia.top * 5.0f * speed * _time;

		mInertia.left += (mInertia.left * -5 * speed * _time);
		mInertia.left += (previousdrag.left * -4.0f * speed * _time);

		mInertia.top += (mInertia.top * -5 * speed * _time);
		mInertia.top += (previousdrag.top * -4.0f * speed * _time);
	}

	void ShaderWobbleNodeAnimator::create()
	{
		mDestroy = false;
	}

	void ShaderWobbleNodeAnimator::destroy()
	{
		mDestroy = true;
	}

	void ShaderWobbleNodeAnimator::attach(MyGUI::ILayerNode* _node)
	{
		_node->castType<MyGUI::RTTLayerNode>()->addLayerNodeAnimation(this);
	}

}
