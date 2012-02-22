/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#include "WobbleNodeAnimator.h"

namespace demo
{

	float squaredDistance(const MyGUI::FloatPoint& _value, const MyGUI::FloatPoint& _other)
	{
		float x1 = _other.left - _value.left;
		float y1 = _other.top - _value.top;

		return x1 * x1 + y1 * y1;
	}

	float getLength(const MyGUI::FloatPoint& _value)
	{
		return (float)sqrt( _value.left * _value.left + _value.top * _value.top );
	}

	void setLength(MyGUI::FloatPoint& _value, float _len)
	{
		float temp = getLength(_value);
		_value.left *= _len / temp;
		_value.top *= _len / temp;
	}

	float squaredLength(const MyGUI::FloatPoint& _value)
	{
		return _value.left * _value.left + _value.top * _value.top;
	}

	WobbleNodeAnimator::WobbleNodeAnimator() :
		mInertiaMode(false),
		mDragStrength(0.001f),
		mResizeStrength(0.0009f),
		mDestroy(true)
	{
	}

	void WobbleNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Property"))
		{
			const std::string& key = node->findAttribute("key");
			const std::string& value = node->findAttribute("value");

			if (key == "DragStrength") mDragStrength = MyGUI::utility::parseFloat(value);
			else if (key == "ResizeStrength") mResizeStrength = MyGUI::utility::parseFloat(value);
		}
	}

	size_t WobbleNodeAnimator::animate(
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
		if (mDestroy)
		{
			return _quad_count;
		}

		// проверяем смещения виджета
		if (mOldCoord.empty())
		{
			mOldCoord = _coord;
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

			addInertia(MyGUI::FloatPoint((float)_coord.width - mOldCoord.width, (float)_coord.height - mOldCoord.height));
		}
		else if (mOldCoord.point() != _coord.point())
		{
			const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
			mInertiaPoint = MyGUI::FloatPoint((float)(point.left - _coord.left) / (float)_coord.width, (float)(point.top - _coord.top) / (float)_coord.height);
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

		_quad_count = tesselation(
			_quad_count,
			_data,
			_texture,
			_info,
			_coord);

		buildQuadVertex(_data);

		return _quad_count;
	}

	void WobbleNodeAnimator::addInertia(const MyGUI::FloatPoint& _value)
	{
		const float clampFactor = 50.0f;
		mInertia = mInertia + _value;
		if ( getLength(mInertia) > clampFactor )
			setLength(mInertia, clampFactor);
	}

	void WobbleNodeAnimator::addTime(float _time)
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

	void WobbleNodeAnimator::buildQuadVertex(MyGUI::VectorQuadData& _data)
	{
		int count_w = getCountX();
		int count_h = getCountY();

		for (int rx = 0; rx < count_w + 1; rx++)
		{
			for (int ry = 0; ry < count_h + 1; ry++)
			{
				MyGUI::FloatPoint point((float)rx / (float)count_w, (float)ry / (float)count_h);

				float drageffect = 0;
				if (mInertiaMode)
				{
					float drageffect1 = squaredDistance(point, MyGUI::FloatPoint(0, 0)) * mResizeStrength;
					float drageffect2 = squaredDistance(point, MyGUI::FloatPoint(1, 1)) * mResizeStrength;

					drageffect = std::min(drageffect1, drageffect2);
				}
				else
				{
					drageffect = squaredDistance(mInertiaPoint, point) * mDragStrength;
				}

				float fx = getLeft() + getWidth() * point.left;
				float fy = getTop() + getHeight() * point.top;

				MyGUI::FloatPoint vert(fx + (-mDragOffset.left) * drageffect, fy + mDragOffset.top * drageffect);

				if (rx < count_w && ry < count_h)
				{
					_data[rx + ry* count_w].vertex[MyGUI::QuadData::CornerLT].x = vert.left;
					_data[rx + ry* count_w].vertex[MyGUI::QuadData::CornerLT].y = vert.top;
				}

				if (rx > 0 && ry > 0)
				{
					_data[(rx-1) + (ry-1)*count_w].vertex[MyGUI::QuadData::CornerRB].x = vert.left;
					_data[(rx-1) + (ry-1)*count_w].vertex[MyGUI::QuadData::CornerRB].y = vert.top;
				}

				if (rx > 0 && ry < count_h)
				{
					_data[(rx-1) + ry* count_w].vertex[MyGUI::QuadData::CornerRT].x = vert.left;
					_data[(rx-1) + ry* count_w].vertex[MyGUI::QuadData::CornerRT].y = vert.top;
				}

				if (rx < count_w && ry > 0)
				{
					_data[rx + (ry-1)*count_w].vertex[MyGUI::QuadData::CornerLB].x = vert.left;
					_data[rx + (ry-1)*count_w].vertex[MyGUI::QuadData::CornerLB].y = vert.top;
				}
			}
		}
	}

	void WobbleNodeAnimator::create()
	{
		mDestroy = false;
	}

	void WobbleNodeAnimator::destroy()
	{
		mDestroy = true;
	}

	void WobbleNodeAnimator::attach(MyGUI::ILayerNode* _node)
	{
		_node->castType<MyGUI::RTTLayerNode>()->addLayerNodeAnimation(this);
	}

}
