/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "WobbleNodeAnimator.h"
#include "MyGUI_RTTLayerNode.h"

namespace demo
{

	float squaredDistance(const MyGUI::FloatPoint& _value, const MyGUI::FloatPoint& _other)
	{
		float x1 = _other.left-_value.left;
		float y1 = _other.top-_value.top;

		return x1*x1 + y1*y1;				
	}

	float getLength(const MyGUI::FloatPoint& _value)
	{
		return (float)sqrt( _value.left*_value.left + _value.top*_value.top );
	}

	void setLength(MyGUI::FloatPoint& _value, float _len)
	{
		float temp = getLength(_value);
		_value.left *= _len / temp;
		_value.top *= _len / temp;
	}

	float squaredLength(const MyGUI::FloatPoint& _value)
	{
		return _value.left*_value.left + _value.top*_value.top;
	}

	WoobleNodeAnimator::WoobleNodeAnimator() :
		mInertiaMode(false),
		mNeedUpdate(false),
		mDragStrength(0.001f),
		mResizeStrength(0.0009f),
		mAlpha(1),
		mColour(0xFFFFFFFF)
	{
	}

	void WoobleNodeAnimator::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
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

	size_t WoobleNodeAnimator::animate(
		bool _update,
		float _time,
		MyGUI::IVertexBuffer* _buffer,
		MyGUI::ITexture* _texture,
		const MyGUI::RenderTargetInfo& _info,
		const MyGUI::IntCoord& _coord
		)
	{
		// первый раз ставим серединку
		/*if (mOldCoord.empty() && !_coord.empty())
		{
			mOldCoord.left = _coord.left + _coord.width / 2;
			mOldCoord.top = _coord.top + _coord.height / 2;
		}*/

		// проверяем смещения виджета
		if (mOldCoord.size() != _coord.size())
		{
			mInertiaMode = true;

			addInertia(MyGUI::FloatPoint(_coord.width - mOldCoord.width, _coord.height-mOldCoord.height));
		}
		else if (mOldCoord.point() != _coord.point())
		{
			const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
			mInertiaPoint = MyGUI::FloatPoint((float)(point.left - _coord.left) / (float)_coord.width , (float)(point.top - _coord.top) / (float)_coord.height);
			mInertiaMode = false;

			addInertia(MyGUI::FloatPoint(_coord.left-mOldCoord.left, _coord.top-mOldCoord.top));
		}

		mOldCoord = _coord;

		addTime(_time);

		bool anim_update = squaredLength(mDragOffset) >= 0.3f;

		int count = MyGUI::VertexQuad::VertexCount;

		// анимация продолжается
		if (anim_update)
		{
			const int count_w = 16;
			const int count_h = 16;
			count = count_w * count_h * MyGUI::VertexQuad::VertexCount;

			// запрашивам нужный размер вершин
			_buffer->setVertextCount(count);

			MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)_buffer->lock();

			float vertex_z = _info.maximumDepth;

			float vertex_left = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
			float vertex_top = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);

			float vertex_width = (_info.pixScaleX * (float)_coord.width * 2);
			float vertex_height = -(_info.pixScaleY * (float)_coord.height * 2);

			float texture_u = (float)_coord.width / (float)_texture->getWidth();
			float texture_v = (float)_coord.height / (float)_texture->getHeight();

			buildQuadVertex(
				MyGUI::FloatCoord(vertex_left, vertex_top, vertex_width, vertex_height), 
				vertex_z,
				quad,
				count_w,
				count_h,
				texture_u,
				texture_v,
				_info.rttFlipY
				);

			_buffer->unlock();

			mNeedUpdate = true;
		}
		// испорчен буфер
		else if (_update || mNeedUpdate)
		{
			// запрашивам нужный размер вершин
			_buffer->setVertextCount(count);

			MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)_buffer->lock();

			float vertex_z = _info.maximumDepth;

			float vertex_left = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
			float vertex_top = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);
			float vertex_right = vertex_left + (_info.pixScaleX * (float)_coord.width * 2);
			float vertex_bottom = vertex_top - (_info.pixScaleY * (float)_coord.height * 2);

			float texture_u = (float)_coord.width / (float)_texture->getWidth();
			float texture_v = (float)_coord.height / (float)_texture->getHeight();
			float texture_v2 = 0;
			if (_info.rttFlipY)
			{
				texture_v = 1 - texture_v;
				texture_v2 = 1;
			}

			quad->set(
				vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z,
				0, texture_v2, texture_u, texture_v, mColour);

			_buffer->unlock();

			mNeedUpdate = false;
		}

		return count;
	}

	void WoobleNodeAnimator::addInertia(const MyGUI::FloatPoint& _value)
	{
		const float clampFactor = 50.0f;
		mInertia = mInertia + _value;
		if ( getLength(mInertia) > clampFactor )
			setLength(mInertia, clampFactor);
	}

	void WoobleNodeAnimator::addTime(float _time)
	{
		if (mAlpha < 1)
		{
			float alpha = mAlpha + _time * 3;
			if (alpha >= 1) alpha = 1;
			mAlpha = alpha;
			mColour = 0xFFFFFF | ((unsigned int)(mAlpha * 255.0) << 24);
			mNeedUpdate = true;
		}

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

	void WoobleNodeAnimator::buildQuadVertex(const MyGUI::FloatCoord& _coord, float _z, MyGUI::VertexQuad* _quads, int _count_w, int _count_h, float _u, float _v, bool _flipY)
	{
		for (int rx=0; rx<_count_w+1; rx++)
		{
			for (int ry=0; ry<_count_h+1; ry++)
			{
				MyGUI::FloatPoint point((float)rx / (float)_count_w, (float)ry / (float)_count_h);

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

				float fx = _coord.left + _coord.width * point.left;
				float fy = _coord.top + _coord.height * point.top;

				MyGUI::FloatPoint vert(fx + (-mDragOffset.left) * drageffect, fy + mDragOffset.top * drageffect);

				float u = point.left * _u;
				float v = point.top * _v;

				if (_flipY) v = 1 - v;

				MyGUI::Vertex vertex;
				vertex.set(vert.left, vert.top, _z, u, v, mColour );

				if (rx < _count_w && ry < _count_h)
				{
					_quads[rx + ry*_count_w].vertex[MyGUI::VertexQuad::CornerLT] = vertex;
				}

				if (rx > 0 && ry > 0)
				{
					_quads[(rx-1) + (ry-1)*_count_w].vertex[MyGUI::VertexQuad::CornerRB] = vertex;
				}

				if (rx > 0 && ry < _count_h)
				{
					_quads[(rx-1) + ry*_count_w].vertex[MyGUI::VertexQuad::CornerRT] = vertex;
					_quads[(rx-1) + ry*_count_w].vertex[MyGUI::VertexQuad::CornerRT2] = vertex;
				}

				if (rx < _count_w && ry > 0)
				{
					_quads[rx + (ry-1)*_count_w].vertex[MyGUI::VertexQuad::CornerLB] = vertex;
					_quads[rx + (ry-1)*_count_w].vertex[MyGUI::VertexQuad::CornerLB2] = vertex;
				}
			}
		}
	}

	void WoobleNodeAnimator::create()
	{
		mAlpha = 0;
	}

	void WoobleNodeAnimator::destroy()
	{
	}

}

